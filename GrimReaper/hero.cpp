#include "hero.h"

//const string Hero::HERO_ANIM_UP = "moveUp";
//const string Hero::HERO_ANIM_DOWN = "moveDown";
//const string Hero::HERO_ANIM_LEFT = "moveLeft";
//const string Hero::HERO_ANIM_RIGHT = "moveRight";
//const string Hero::HERO_ANIM_IDLE_UP = "idleUp";
//const string Hero::HERO_ANIM_IDLE_DOWN = "idleDown";
//const string Hero::HERO_ANIM_IDLE_LEFT = "idleUp";
//const string Hero::HERO_ANIM_IDLE_RIGHT = "idleUp";
//const string Hero::HERO_SLASH_ANIM_UP = "slashUp";
//const string Hero::HERO_SLASH_ANIM_DOWN = "slashDown";
//const string Hero::HERO_SLASH_ANIM_LEFT = "slashLeft";
//const string Hero::HERO_SLASH_ANIM_RIGHT = "slashRight";
//const string Hero::HERO_DASH_ANIM_UP = "hitUp";
//const string Hero::HERO_DASH_ANIM_DOWN = "hitDown";
//const string Hero::HERO_DASH_ANIM_LEFT = "hitLeft";
//const string Hero::HERO_DASH_ANIM_RIGHT = "hitRight";
//const string Hero::HERO_ANIM_DIE = "die";

const string Hero::HERO_ANIM_UP = "up";
const string Hero::HERO_ANIM_DOWN = "down";
const string Hero::HERO_ANIM_LEFT = "left";
const string Hero::HERO_ANIM_RIGHT = "right";
const string Hero::HERO_ANIM_IDLE_UP = "idleUp";
const string Hero::HERO_ANIM_IDLE_DOWN = "idleDown";
const string Hero::HERO_ANIM_IDLE_LEFT = "idleLeft";
const string Hero::HERO_ANIM_IDLE_RIGHT = "idleRight";
const string Hero::HERO_SLASH_ANIM_UP = "slashUp";
const string Hero::HERO_SLASH_ANIM_DOWN = "slashDown";
const string Hero::HERO_SLASH_ANIM_LEFT = "slashLeft";
const string Hero::HERO_SLASH_ANIM_RIGHT = "slashRight";
const string Hero::HERO_DASH_ANIM_UP = "dashUp";
const string Hero::HERO_DASH_ANIM_DOWN = "dashDown";
const string Hero::HERO_DASH_ANIM_LEFT = "dashLeft";
const string Hero::HERO_DASH_ANIM_RIGHT = "dashRight";
const string Hero::HERO_ANIM_DIE = "die";


const int Hero::HERO_STATE_IDLE = 0;
const int Hero::HERO_STATE_MOVE = 1;
const int Hero::HERO_STATE_SLASH = 2;
const int Hero::HERO_STATE_DASH = 3 ;
const int Hero::HERO_STATE_DEAD = 4;

Hero::Hero(AnimationSet *animSet) {
	this->animSet = animSet;
	type = "hero";

	//Default hero values
	x = Globals::ScreenWidth / 2;
	y = Globals::ScreenHeight / 2;
	moveSpeed = 0;
	moveSpeedMax = 80;
	hp = hpMax = 20;
	damage = 0;
	collisionBoxW = 20;
	collisionBoxH = 24;
	collisionBoxYOffset = -20;
	direction = DIR_DOWN;

	changeAnimation(HERO_STATE_IDLE, true);
	updateCollisionBox();
}

void Hero::update() {
	//Check if dead
	if (hp < 1 && state != HERO_STATE_DEAD) {
		changeAnimation(HERO_STATE_DEAD, true);
		moving = false;
		die();
	}
	
	updateCollisionBox(); 
	updateMovement();
	updateCollisions();
	updateHitBox();
	updateDamages();
	updateAnimation();
	updateInvincibleTimer();
}

void Hero::slash() {
	if (hp > 0 && (state == HERO_STATE_MOVE || state == HERO_STATE_IDLE)) {
		moving = false;
		frameTimer = 0;

		changeAnimation(HERO_STATE_SLASH, true);
		//TODO add attack sound!
	}
}

void Hero::dash() {
	if (hp > 0 && (state == HERO_STATE_MOVE || state == HERO_STATE_IDLE)) {
		moving = false;
		frameTimer = 0;

		//Push the hero in the direction they are travelling
		slideAngle = angle;
		slideAmount = 300;
		invincibleTimer = 0.1;

		changeAnimation(HERO_STATE_DASH, true);
		//TODO add dash sound!
	}
}

void Hero::die() {
	moving = false;

	changeAnimation(HERO_STATE_DEAD, true);
}

void Hero::revive() {
	hp = hpMax;
	changeAnimation(HERO_STATE_IDLE, true);
	moving = false;
	x = Globals::ScreenWidth / 2;
	y = Globals::ScreenHeight / 2;
	slideAmount = 0;
}

void Hero::changeAnimation(int newState, bool resetFrameToBeginning) {
	state = newState;

	if (state == HERO_STATE_IDLE){
		if (direction == DIR_DOWN)
			currentAnim = animSet->getAnimation(HERO_ANIM_IDLE_DOWN);
		else if (direction == DIR_UP)
			currentAnim = animSet->getAnimation(HERO_ANIM_IDLE_UP);
		else if (direction == DIR_LEFT)
			currentAnim = animSet->getAnimation(HERO_ANIM_IDLE_LEFT);
		else if (direction == DIR_RIGHT)
			currentAnim = animSet->getAnimation(HERO_ANIM_IDLE_RIGHT);
	}
	else if (state == HERO_STATE_MOVE){
		if (direction == DIR_DOWN)
			currentAnim = animSet->getAnimation(HERO_ANIM_DOWN);
		else if (direction == DIR_UP)
			currentAnim = animSet->getAnimation(HERO_ANIM_UP);
		else if (direction == DIR_LEFT)
			currentAnim = animSet->getAnimation(HERO_ANIM_LEFT);
		else if (direction == DIR_RIGHT)
			currentAnim = animSet->getAnimation(HERO_ANIM_RIGHT);
	}
	else if (state == HERO_STATE_SLASH){
		if (direction == DIR_DOWN)
			currentAnim = animSet->getAnimation(HERO_SLASH_ANIM_DOWN);
		else if (direction == DIR_UP)
			currentAnim = animSet->getAnimation(HERO_SLASH_ANIM_UP);
		else if (direction == DIR_LEFT)
			currentAnim = animSet->getAnimation(HERO_SLASH_ANIM_LEFT);
		else if (direction == DIR_RIGHT)
			currentAnim = animSet->getAnimation(HERO_SLASH_ANIM_RIGHT);
	}
	else if (state == HERO_STATE_DASH){
		if (direction == DIR_DOWN)
			currentAnim = animSet->getAnimation(HERO_DASH_ANIM_DOWN);
		else if (direction == DIR_UP)
			currentAnim = animSet->getAnimation(HERO_DASH_ANIM_UP);
		else if (direction == DIR_LEFT)
			currentAnim = animSet->getAnimation(HERO_DASH_ANIM_LEFT);
		else if (direction == DIR_RIGHT)
			currentAnim = animSet->getAnimation(HERO_DASH_ANIM_RIGHT);
	}
	else if (state == HERO_STATE_DEAD){
		currentAnim = animSet->getAnimation(HERO_ANIM_DIE);
	}

	if (resetFrameToBeginning)
		currentFrame = currentAnim->getFrame(0);
	else
		currentFrame = currentAnim->getFrame(currentFrame->frameNumber);
}
void Hero::updateAnimation() {
	if (currentFrame == NULL || currentAnim == NULL)
		return; 

	if (state == HERO_STATE_MOVE && !moving) {
		changeAnimation(HERO_STATE_IDLE, true);
	}

	if (state != HERO_STATE_MOVE && moving) {
		changeAnimation(HERO_STATE_MOVE, true);
	}

	frameTimer += TimeState::timeState.dT;

	if (frameTimer >= currentFrame->duration) {
		if (currentFrame->frameNumber == currentAnim->getEndFrameNumber()) {
			if (state == HERO_STATE_SLASH || state == HERO_STATE_DASH) {
				//Change back to moving state
				changeAnimation(HERO_STATE_MOVE, true);
			}
			else if (state == HERO_STATE_DEAD && hp > 0) {
				//Revive after death with HP gain
				changeAnimation(HERO_STATE_MOVE, true);
			}
			else {
				//Reset animation
				currentFrame = currentAnim->getFrame(0);
			}
		}
		else {
			//Move onto the next frame in this animation
			currentFrame = currentAnim->getNextFrame(currentFrame);
		}
		frameTimer = 0;
	}
}

void Hero::updateDamages() {
	if (active && hp > 0 && invincibleTimer <= 0){
		for (auto entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++) {
			if ((*entity)->active && (*entity)->type == "enemy") {
				LivingEntity* enemy = (LivingEntity*)(*entity);

				bool isCheck = Entity::checkCollision(collisionBox, enemy->hitBox);

				if (isCheck && enemy->damage > 0) {
					hp -= enemy->damage;

					if (hp > 0) {
						invincibleTimer = 0.3;
						//TODO play getting hit sound
					}

					slideAngle = Entity::angleBetweenTwoEntities((*entity), this);
					slideAmount = 200;
				}
			}
		}
	}
}