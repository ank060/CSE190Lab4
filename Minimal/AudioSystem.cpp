#include "AudioSystem.h"

#include <stdio.h>

#include <iostream>
#include <fstream>
#include <cstring>

AudioSystem::AudioSystem()
{
	FMOD::System_Create(&soundSystem);
	soundSystem->init(32, FMOD_INIT_NORMAL, extraDriverData);

	registerSound("ding", "./audio/ding.wav", false);
	registerSound("ambient", "./audio/ambient_noise.wav", true);
	registerSound("hand", "./audio/hand_noise.wav", false);
	registerSound("hit", "./audio/hit_in_head.wav", false);
	registerSound("hit_ball", "./audio/ball_hit.wav", false);
	registerSound("space_ambient", "./audio/space1.wav", true);
	registerSound("space_music", "./audio/space2.wav", true);
	registerSound("pop", "./audio/pop.wav", false);
	registerSound("tick", "./audio/tick.wav", false);
	registerSound("grab_ball_object", "./audio/grab_ball_object.wav", false);
}

void AudioSystem::registerSound(std::string name, std::string path, bool loop)
{
	FMOD::Sound* sound;
	soundSystem->createSound(path.c_str(), FMOD_DEFAULT, 0, &sound);
	if (!loop)
	{
		sound->setMode(FMOD_LOOP_OFF);
	}
	else
	{
		sound->setMode(FMOD_LOOP_NORMAL);
	}
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
	soundSystem->playSound(getSound(name), 0, false, &channel1);
}

void AudioSystem::playVariedSound(std::string name, float variation)
{
	float value = ((rand() % 100) / 50.0f) - 1;
	channel2->setPitch(1 + value * variation);
	soundSystem->playSound(getSound(name), 0, false, &channel2);
}

void AudioSystem::playGoalSound()
{
	playVariedSound("pop", 0.1);
}
