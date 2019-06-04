#include "AudioSystem.h"

#include <AL/al.h>
#include <AL/alc.h>

#include <stdio.h>

#include <iostream>
#include <fstream>
#include <cstring>
//check big vs little endian machine
static bool IsBigEndian(void)
{
	int a = 1;
	return !((char*)&a)[0];
}

static int ConvertToInt(char* buffer, int len)
{
	int a = 0;

	if (!IsBigEndian())
	{
		for (int i = 0; i < len; ++i)
		{
			((char*)&a)[i] = buffer[i];
		}
	}
	else
	{
		for (int i = 0; i < len; ++i)
		{
			((char*)&a)[3 - i] = buffer[i];
		}
	}

	return a;
}

//Location and size of data is found here: http://www.topherlee.com/software/pcm-tut-wavformat.html
static char* LoadWAV(std::string filename, int& channels, int& sampleRate, int& bps, int& size)
{
	char buffer[4];

	std::ifstream in(filename.c_str());
	in.read(buffer, 4);

	if (strncmp(buffer, "RIFF", 4) != 0)
	{
		std::cout << "Error here, not a valid WAV file, RIFF not found in header\n This was found instead: "
			<< buffer[0] << buffer[1] << buffer[2] << buffer[3] << std::endl;
	}

	in.read(buffer, 4);//size of file. Not used. Read it to skip over it.  

	in.read(buffer, 4);//Format, should be WAVE

	if (strncmp(buffer, "WAVE", 4) != 0)
	{
		std::cout << "Error here, not a valid WAV file, WAVE not found in header.\n This was found instead: "
			<< buffer[0] << buffer[1] << buffer[2] << buffer[3] << std::endl;
	}

	in.read(buffer, 4);//Format Space Marker. should equal fmt (space)

	if (strncmp(buffer, "fmt ", 4) != 0)
	{
		std::cout << "Error here, not a valid WAV file, Format Marker not found in header.\n This was found instead: "
			<< buffer[0] << buffer[1] << buffer[2] << buffer[3] << std::endl;
	}

	in.read(buffer, 4);//Length of format data. Should be 16 for PCM, meaning uncompressed.

	if (ConvertToInt(buffer, 4) != 16)
	{
		std::cout << "Error here, not a valid WAV file, format length wrong in header.\n This was found instead: "
			<< ConvertToInt(buffer, 4) << std::endl;
	}

	in.read(buffer, 2);//Type of format, 1 = PCM

	if (ConvertToInt(buffer, 2) != 1)
	{
		std::cout << "Error here, not a valid WAV file, file not in PCM format.\n This was found instead: "
			<< ConvertToInt(buffer, 4) << std::endl;
	}

	in.read(buffer, 2);//Get number of channels. 

	//Assume at this point that we are dealing with a WAV file. This value is needed by OpenAL
	channels = ConvertToInt(buffer, 2);

	in.read(buffer, 4);//Get sampler rate. 

	sampleRate = ConvertToInt(buffer, 4);

	//Skip Byte Rate and Block Align. Maybe use later?
	in.read(buffer, 4);//Block Align
	in.read(buffer, 2);//ByteRate

	in.read(buffer, 2);//Get Bits Per Sample

	bps = ConvertToInt(buffer, 2);

	//Skip character data, which marks the start of the data that we care about. 
	in.read(buffer, 4);//"data" chunk. 

	in.read(buffer, 4); //Get size of the data

	size = ConvertToInt(buffer, 4);

	if (size < 0)
	{
		std::cout << "Error here, not a valid WAV file, size of file reports 0.\n This was found instead: "
			<< size << std::endl;
	}

	char* data = new char[size];

	in.read(data, size);//Read audio data into buffer, return.

	in.close();

	return data;
}

const unsigned int NUM_BUFFERS = 1;
const unsigned int NUM_SOURCES = 1;

ALuint buffers[NUM_BUFFERS];
ALuint source[NUM_SOURCES];

ALCdevice *device;
ALCcontext *context;

AudioSystem::AudioSystem()
{
	// Init openAL
	device = alcOpenDevice(NULL);
	context = alcCreateContext(device, NULL);
	
	// Clear Error Code (so we can catch any new errors)
	alGetError();

	// Create the audio
	ALuint error;

	// Create the buffers
	int channel, sampleRate, bps, size;
	char* data = LoadWAV("./audio/ding.wav", channel, sampleRate, bps, size);
	unsigned int format;
	alGenBuffers(1, &buffers[0]);
	if (channel == 1)
	{
		if (bps == 8)
		{
			format = AL_FORMAT_MONO8;
		}
		else {
			format = AL_FORMAT_MONO16;
		}
	}
	else {
		if (bps == 8)
		{
			format = AL_FORMAT_STEREO8;
		}
		else {
			format = AL_FORMAT_STEREO16;
		}
	}
	alBufferData(buffers[0], format, data, size, sampleRate);

	// Generate the sources
	alGenSources(NUM_SOURCES, source);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		printf("alGenSources : %d", error);
		return;
	}

	alSourcei(source[0], AL_BUFFER, buffers[0]);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		printf("alSourcei : %d", error);
		return;
	}
}

AudioSystem::~AudioSystem()
{
	alDeleteSources(NUM_SOURCES, source);
	alDeleteBuffers(NUM_BUFFERS, buffers);
	device = alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
}


void AudioSystem::playGoalSound()
{
	alSourcePlay(source[0]);

}
