#include "arena.h"

Arena::Arena(AnimationSet *animSet) {
	this->animSet = animSet;
	solid = true;
	collisionBoxW = 32;
	collisionBoxH = 32;
	collisionBoxYOffset = -16;

	updateCollisionBox();
	changeAnimation(0, false);

}

void Arena::update() {
	updateCollisionBox();

	if (currentFrame == NULL || currentAnim == NULL)
		return;

	frameTimer += TimeState::timeState.dT;

	if (frameTimer >= currentFrame->duration){
		currentFrame = currentAnim->getNextFrame(currentFrame);
		frameTimer = 0;
	}
}

void Arena::changeAnimation(int newState, bool resetFrameToBeginning) {
	currentAnim = animSet->getAnimation("wall");
	currentFrame = currentAnim->getFrame(0);
}