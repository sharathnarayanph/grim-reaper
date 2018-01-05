#ifndef ANIMATIONSET_H
#define ANIMATIONSET_H

#include "animation.h"

using namespace std;

class AnimationSet {
public:
	string imageName;
	SDL_Texture* spriteSheet; 
	SDL_Texture* whiteSpriteSheet = NULL; 
	list<Animation> animations;

	Animation* getAnimation(string name);

	~AnimationSet(); 
};

#endif