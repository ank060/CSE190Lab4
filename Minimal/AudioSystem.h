#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#include <map>
#include <fmod/fmod.hpp>

class AudioSystem {
	std::map<std::string, FMOD::Sound*> sounds;
	FMOD::System* soundSystem;
	FMOD::Channel* channel = 0;
	void* extraDriverData = 0;

public:
	AudioSystem();
	~AudioSystem();

	void registerSound(std::string name, std::string path);
	FMOD::Sound* getSound(std::string name);

	void update();
	void playGoalSound();
};

#endif
