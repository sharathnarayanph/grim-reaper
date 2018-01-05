#ifndef LIVINGENTITY_H
#define LIVINGENTITY_H

#include "entity.h"

class LivingEntity : public Entity{
public:
	int hp, hpMax;
	int damage = 0;
	SDL_Rect hitBox; //Defines damage area

	float invincibleTimer = 0; // Can't be hurt when > 0

	virtual void updateHitBox();
	virtual void updateDamages() = 0; 
	virtual void die() = 0; 
	virtual void updateInvincibleTimer();

	void draw();
};

#endif