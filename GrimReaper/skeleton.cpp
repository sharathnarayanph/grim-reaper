#include "skeleton.h"

//const string Skeleton::SKELETON_ANIM_UP = "moveUp";
//const string Skeleton::SKELETON_ANIM_DOWN = "moveDown";
//const string Skeleton::SKELETON_ANIM_LEFT = "moveLeft";
//const string Skeleton::SKELETON_ANIM_RIGHT = "moveRight";
//const string Skeleton::SKELETON_ANIM_IDLE_UP = "idleUp";
//const string Skeleton::SKELETON_ANIM_IDLE_DOWN = "idleDown";
//const string Skeleton::SKELETON_ANIM_IDLE_LEFT = "idleLeft";
//const string Skeleton::SKELETON_ANIM_IDLE_RIGHT = "idleRight";
//const string Skeleton::SKELETON_ANIM_ATTACK_UP = "stabUp";
//const string Skeleton::SKELETON_ANIM_ATTACK_DOWN = "stabDown";
//const string Skeleton::SKELETON_ANIM_ATTACK_LEFT = "stabLeft";
//const string Skeleton::SKELETON_ANIM_ATTACK_RIGHT = "stabRight";
//const string Skeleton::SKELETON_ANIM_TELEGRAPH_UP = "hitUp";
//const string Skeleton::SKELETON_ANIM_TELEGRAPH_DOWN = "hitDown";
//const string Skeleton::SKELETON_ANIM_TELEGRAPH_LEFT = "hitLeft";
//const string Skeleton::SKELETON_ANIM_TELEGRAPH_RIGHT = "hitRight";
//const string Skeleton::SKELETON_ANIM_DIE = "die";

const string Skeleton::SKELETON_ANIM_UP = "up";
const string Skeleton::SKELETON_ANIM_DOWN = "down";
const string Skeleton::SKELETON_ANIM_LEFT = "left";
const string Skeleton::SKELETON_ANIM_RIGHT = "right";
const string Skeleton::SKELETON_ANIM_IDLE_UP = "idleUp";
const string Skeleton::SKELETON_ANIM_IDLE_DOWN = "idleDown";
const string Skeleton::SKELETON_ANIM_IDLE_LEFT = "idleLeft";
const string Skeleton::SKELETON_ANIM_IDLE_RIGHT = "idleRight";
const string Skeleton::SKELETON_ANIM_ATTACK_UP = "attackUp";
const string Skeleton::SKELETON_ANIM_ATTACK_DOWN = "attackDown";
const string Skeleton::SKELETON_ANIM_ATTACK_LEFT = "attackLeft";
const string Skeleton::SKELETON_ANIM_ATTACK_RIGHT = "attackRight";
const string Skeleton::SKELETON_ANIM_TELEGRAPH_UP = "telegraphUp";
const string Skeleton::SKELETON_ANIM_TELEGRAPH_DOWN = "telegraphDown";
const string Skeleton::SKELETON_ANIM_TELEGRAPH_LEFT = "telegraphLeft";
const string Skeleton::SKELETON_ANIM_TELEGRAPH_RIGHT = "telegraphRight";
const string Skeleton::SKELETON_ANIM_DIE = "die";

const int Skeleton::SKELETON_STATE_IDLE = 0;
const int Skeleton::SKELETON_STATE_MOVE = 1;
const int Skeleton::SKELETON_STATE_ATTACK = 2;
const int Skeleton::SKELETON_STATE_TELEGRAPH = 3;
const int Skeleton::SKELETON_STATE_DEAD = 4;

const int Skeleton::SKELETON_AI_NORMAL = 0;
const int Skeleton::SKELETON_AI_CHASE = 1;

int Skeleton::killCnt = 0;

Skeleton::Skeleton(AnimationSet *animSet) {
	this->animSet = animSet;

	type = "enemy";

	x = Globals::ScreenWidth / 2;
	y = Globals::ScreenHeight / 2;
	moveSpeed = 0;
	moveSpeedMax = 20;
	hp = hpMax = 10 + (rand() % 20);
	damage = 0;
	collisionBoxW = 18;
	collisionBoxH = 20;
	collisionBox.w = collisionBoxW;
	collisionBox.h = collisionBoxH;

	collisionBoxYOffset = -14;

	direction = DIR_DOWN;
	changeAnimation(SKELETON_STATE_IDLE, true);

	updateCollisionBox();
}

void Skeleton::update() {
	//If about to die
	if (hp < 1 && state != SKELETON_STATE_DEAD){
		changeAnimation(SKELETON_STATE_DEAD, true);
		moving = false;
		die();
	}

	think();
	updateCollisionBox();
	updateMovement();
	updateCollisions();
	updateHitBox();
	updateDamages();
	updateAnimation();
	updateInvincibleTimer();
}

void Skeleton::think() {
	if (state == SKELETON_STATE_IDLE || state == SKELETON_STATE_MOVE){
		thinkTimer -= TimeState::timeState.dT;

		if (thinkTimer <= 0){
			//Reset timer
			thinkTimer = rand() % 5;
			int action = rand() % 10;

			if (action < 3) {
				moving = false;
				aiState = SKELETON_AI_NORMAL;
				changeAnimation(SKELETON_STATE_IDLE, true);
			}
			else
			{
				findNearestTarget();

				//Approach target
				if (target != NULL && target->hp > 0) {
					float dist = Entity::distanceBetweenTwoEntities(this, target);
					//Attack if in range
					if (dist < 100) {
						telegraph(); 
						aiState = SKELETON_AI_NORMAL;

					}
					else {
						aiState = SKELETON_AI_CHASE;
						moving = true;
						changeAnimation(SKELETON_STATE_MOVE, state != SKELETON_STATE_MOVE);
					}
				}
				else {
					//Go idle if no target
					moving = false;
					aiState = SKELETON_AI_NORMAL;
					changeAnimation(SKELETON_STATE_IDLE, true);
				}
			}
			
		}
	}

	//If chasing target
	if (aiState == SKELETON_AI_CHASE && hp > 0 && active){
		angle = Entity::angleBetweenTwoEntities(this, target);

		move(angle);
	}
}

void Skeleton::telegraph() {
	moving = false;
	frameTimer = 0;

	changeAnimation(SKELETON_STATE_TELEGRAPH, true);
}

void Skeleton::attack() {
	moving = false;
	frameTimer = 0;
	slideAmount = 100;
	slideAngle = angle;

	changeAnimation(SKELETON_STATE_ATTACK, true);
}

void Skeleton::die() {
	moving = false;
	state = SKELETON_STATE_DEAD;
	Skeleton::killCnt++;

	changeAnimation(state, true);	
}

void Skeleton::findNearestTarget() {
	target = NULL;

	//Find closest target
	for (auto entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++) {
		if ((*entity)->type == "hero" && (*entity)->active) {
			if (target == NULL)
				target = (LivingEntity*)(*entity); 
			else {
				if (Entity::distanceBetweenTwoEntities(this, (*entity)) < Entity::distanceBetweenTwoEntities(this, target)) {
					target = (LivingEntity*)(*entity);
				}
			}
		}
	}

}
void Skeleton::changeAnimation(int newState, bool resetFrameToBeginning) {
	state = newState;

	if (state == SKELETON_STATE_IDLE) {
		if (direction == DIR_DOWN)
			currentAnim = animSet->getAnimation(SKELETON_ANIM_IDLE_DOWN);
		if (direction == DIR_UP)
			currentAnim = animSet->getAnimation(SKELETON_ANIM_IDLE_UP);
		if (direction == DIR_LEFT)
			currentAnim = animSet->getAnimation(SKELETON_ANIM_IDLE_LEFT);
		if (direction == DIR_RIGHT)
			currentAnim = animSet->getAnimation(SKELETON_ANIM_IDLE_RIGHT);
	}
	else if (state == SKELETON_STATE_MOVE) {
		if (direction == DIR_DOWN)
			currentAnim = animSet->getAnimation(SKELETON_ANIM_DOWN);
		if (direction == DIR_UP)
			currentAnim = animSet->getAnimation(SKELETON_ANIM_UP);
		if (direction == DIR_LEFT)
			currentAnim = animSet->getAnimation(SKELETON_ANIM_LEFT);
		if (direction == DIR_RIGHT)
			currentAnim = animSet->getAnimation(SKELETON_ANIM_RIGHT);
	}
	else if (state == SKELETON_STATE_ATTACK) {
		if (direction == DIR_DOWN)
			currentAnim = animSet->getAnimation(SKELETON_ANIM_ATTACK_DOWN);
		if (direction == DIR_UP)
			currentAnim = animSet->getAnimation(SKELETON_ANIM_ATTACK_UP);
		if (direction == DIR_LEFT)
			currentAnim = animSet->getAnimation(SKELETON_ANIM_ATTACK_LEFT);
		if (direction == DIR_RIGHT)
			currentAnim = animSet->getAnimation(SKELETON_ANIM_ATTACK_RIGHT);
	}
	else if (state == SKELETON_STATE_TELEGRAPH) {
		if (direction == DIR_DOWN) 
			currentAnim = animSet->getAnimation(SKELETON_ANIM_TELEGRAPH_DOWN);
		if (direction == DIR_UP)
			currentAnim = animSet->getAnimation(SKELETON_ANIM_TELEGRAPH_UP);
		if (direction == DIR_LEFT)
			currentAnim = animSet->getAnimation(SKELETON_ANIM_TELEGRAPH_LEFT);
		if (direction == DIR_RIGHT)
			currentAnim = animSet->getAnimation(SKELETON_ANIM_TELEGRAPH_RIGHT);
	}
	else if (state == SKELETON_STATE_DEAD) {
		currentAnim = animSet->getAnimation(SKELETON_ANIM_DIE);
	}

	if (resetFrameToBeginning)
		currentFrame = currentAnim->getFrame(0);
	else
		currentFrame = currentAnim->getFrame(currentFrame->frameNumber);
}

void Skeleton::updateAnimation() {
	if (currentFrame == NULL || currentAnim == NULL)
		return;

	//If in moving state but not moving
	if (state == SKELETON_STATE_MOVE && !moving)
		changeAnimation(SKELETON_STATE_IDLE, true);

	//If idle but still moving
	if (state != SKELETON_STATE_MOVE && moving)
		changeAnimation(SKELETON_STATE_MOVE, true);

	frameTimer += TimeState::timeState.dT;

	if (frameTimer >= currentFrame->duration) {
		//If end of animation
		if (currentFrame->frameNumber == currentAnim->getEndFrameNumber()) {
			if (state == SKELETON_STATE_TELEGRAPH)
				attack();
			else if (state == SKELETON_STATE_ATTACK)
				changeAnimation(SKELETON_STATE_MOVE, true);
			else if (state == SKELETON_STATE_DEAD) {
				frameTimer = 0;

				if (hp > 0)
					changeAnimation(SKELETON_STATE_MOVE, true);
				else
					active = false; 
			}
			else
				currentFrame = currentAnim->getFrame(0);
		}
		else
			currentFrame = currentAnim->getNextFrame(currentFrame);

		frameTimer = 0;
	}
}

void Skeleton::updateDamages() {
	if (active && hp > 0 && invincibleTimer <= 0) {
		for (auto entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++) {
			if ((*entity)->active && (*entity)->type == "hero") {
				LivingEntity* enemy = (LivingEntity*)(*entity);
				//Enemey hit damage
				if (enemy->damage > 0 && Entity::checkCollision(collisionBox, enemy->hitBox)){
					hp -= enemy->damage;

					if (hp > 0){
						//TODO play hurt sound
						invincibleTimer = 0.1;
					}
					//Angle between entities
					slideAngle = Entity::angleBetweenTwoEntities((*entity), this);
					slideAmount = 300;
				}
			}
		}
	}
}