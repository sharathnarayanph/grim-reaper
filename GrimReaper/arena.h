#ifndef ARENA_H
#define ARENA_H

#include "entity.h"

class Arena : public Entity{
public:
	Arena(AnimationSet *animSet);

	void update();

	void changeAnimation(int newState, bool resetFrameToBeginning);

	void updateCollisions(){

	}
};

#endif