#ifndef ANIMATIONSET_H
#define ANIMATIONSET_H

#include "animation.h"

class AnimationSet {
public:
	std::string imageName;
	SDL_Texture* spriteSheet; 
	SDL_Texture* whiteSpriteSheet = NULL; 
	std::list<Animation> animations;

	Animation* getAnimation(string name);

	void loadAnimationSet(std::string fileName, std::list<FrameSetType> &types, bool setColourKey = false, int transparentPixelIndex = 0, bool createWhiteTexture = false);

	~AnimationSet(); 
};

#endif