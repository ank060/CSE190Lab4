#include "AudioSystem.h"

#include <stdio.h>

#include <iostream>
#include <fstream>
#include <cstring>

AudioSystem::AudioSystem()
{
	FMOD::System_Create(&soundSystem);
	soundSystem->init(32, FMOD_INIT_NORMAL, extraDriverData);

	registerSound("ding", "./audio/ding.wav");
}

void AudioSystem::registerSound(std::string name, std::string path)
{
	FMOD::Sound* sound;
	soundSystem->createSound(path.c_str(), FMOD_DEFAULT, 0, &sound);
	sound->setMode(FMOD_LOOP_OFF);
	sounds[name] = sound;
}

FMOD::Sound* AudioSystem::getSound(std::string name)
{
	return sounds[name];
}

AudioSystem::~AudioSystem()
{
	for (auto it = sounds.begin(), et = sounds.end(); it != et; ++it)
	{
		it->second->release();
	}
	sounds.clear();
	soundSystem->close();
	soundSystem->release();
}

void AudioSystem::update()
{
	soundSystem->update();
}

void AudioSystem::playSound(std::string name)
{
	soundSystem->playSound(getSound(name), 0, false, &channel);
}

void AudioSystem::playGoalSound()
{
	soundSystem->playSound(getSound("ding"), 0, false, &channel);
}
