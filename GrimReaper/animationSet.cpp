#include "animationSet.h"

Animation* AnimationSet::getAnimation(string name) {
	for (list<Animation>::iterator animation = animations.begin(); animation != animations.end(); animation++) {
		Animation* anim = &(*animation);

		if (name == anim->name)
			return anim;
	}

	return NULL;
}

AnimationSet::~AnimationSet() {
	cleanup(spriteSheet);
	if (whiteSpriteSheet != NULL)
		cleanup(whiteSpriteSheet);
}