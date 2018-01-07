#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include "SDL_mixer.h"
#include <string>
#include <list>

using namespace std;

struct soundListing { 
	Mix_Chunk* sound;
	string name;
};

class SoundManager {
public:
	static SoundManager soundManager;
	list<soundListing> sounds;

	SoundManager();
	
	void loadSound(string name, string file);
	void playSound(string name);

	

	~SoundManager();
};

#endif