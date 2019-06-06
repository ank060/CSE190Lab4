#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#include <map>
#include <fmod/fmod.hpp>

class AudioSystem {
	std::map<std::string, FMOD::Sound*> sounds;
	FMOD::System* soundSystem;
	FMOD::Channel* channel1 = 0;
	FMOD::Channel* channel2 = 0;
	void* extraDriverData = 0;

public:
	AudioSystem();
	~AudioSystem();

	void registerSound(std::string name, std::string path, bool loop);
	FMOD::Sound* getSound(std::string name);

	void update();
	void playVariedSound(std::string name, float variation);
	void playSound(std::string name);
	void playGoalSound();
};

#endif
