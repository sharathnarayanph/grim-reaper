#include "living_entity.h"

void LivingEntity::updateHitBox() {
	damage = 0;

	FrameSetRect* hitBoxes = (FrameSetRect*)FrameSetUtil::findSetByName("hitBox", currentFrame->frameData);
	if (hitBoxes != NULL && hitBoxes->frameSetCount() > 0) {
		//Update hitbox
		SDL_Rect hb = hitBoxes->data.front();
		hitBox.x = x - currentFrame->offSet.x + hb.x;
		hitBox.y = y - currentFrame->offSet.y + hb.y;
		hitBox.w = hb.w;
		hitBox.h = hb.h;

		//Update damage
		FrameSetVal* damages = (FrameSetVal*)FrameSetUtil::findSetByName("damage", currentFrame->frameData);
		if (damages != NULL && damages->frameSetCount() > 0) {
			damage = damages->data.front();
		}
	}
}

void LivingEntity::updateInvincibleTimer() {
	if (invincibleTimer > 0){
		invincibleTimer -= TimeState::timeState.dT;
	}
}

void LivingEntity::draw() {
	if (currentFrame != NULL && active) 
		currentFrame->Draw(animSet->spriteSheet, x, y);

	//Draw collsionBox
	if (solid && Globals::debugging) {
		SDL_SetRenderDrawColor(Globals::renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(Globals::renderer, &lastCollisionBox);

		SDL_SetRenderDrawColor(Globals::renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(Globals::renderer, &collisionBox);

		SDL_SetRenderDrawColor(Globals::renderer, 255, 0, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(Globals::renderer, &hitBox);
	}
}