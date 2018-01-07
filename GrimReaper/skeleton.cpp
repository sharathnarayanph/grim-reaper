#include "skeleton.h"

const string Skeleton::SKELETON_ANIM_UP = "moveUp";
const string Skeleton::SKELETON_ANIM_DOWN = "moveDown";
const string Skeleton::SKELETON_ANIM_LEFT = "moveLeft";
const string Skeleton::SKELETON_ANIM_RIGHT = "moveRight";
const string Skeleton::SKELETON_ANIM_IDLE_UP = "idleUp";
const string Skeleton::SKELETON_ANIM_IDLE_DOWN = "idleDown";
const string Skeleton::SKELETON_ANIM_IDLE_LEFT = "idleLeft";
const string Skeleton::SKELETON_ANIM_IDLE_RIGHT = "idleRight";
const string Skeleton::SKELETON_ANIM_ATTACK_UP = "stabUp";
const string Skeleton::SKELETON_ANIM_ATTACK_DOWN = "stabDown";
const string Skeleton::SKELETON_ANIM_ATTACK_LEFT = "stabLeft";
const string Skeleton::SKELETON_ANIM_ATTACK_RIGHT = "stabRight";
const string Skeleton::SKELETON_ANIM_TELEGRAPH_UP = "hitUp";
const string Skeleton::SKELETON_ANIM_TELEGRAPH_DOWN = "hitDown";
const string Skeleton::SKELETON_ANIM_TELEGRAPH_LEFT = "hitLeft";
const string Skeleton::SKELETON_ANIM_TELEGRAPH_RIGHT = "hitRight";
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

	//defaults
	x = Globals::ScreenWidth / 2;
	y = Globals::ScreenHeight / 2;
	moveSpeed = 0;
	moveSpeedMax = 20;
	hp = hpMax = 10 + (rand() % 20); //10-29
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
void Skeleton::update(){
	//check if died
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
void Skeleton::think(){
	if (state == SKELETON_STATE_IDLE || state == SKELETON_STATE_MOVE){
		thinkTimer -= TimeState::timeState.dT;
		//time to choose an action
		if (thinkTimer <= 0){
			//reset the timer
			thinkTimer = rand() % 5;//0-5seconds
			int action = rand() % 10; //0-9

			if (action < 3){
				moving = false;
				aiState = SKELETON_AI_NORMAL;
				changeAnimation(SKELETON_STATE_IDLE, true);
			}
			else
			{
				findNearestTarget();
				//found a target and its alive, lets get 'em
				if (target != NULL && target->hp > 0){
					float dist = Entity::distanceBetweenTwoEntities(this, target);
					//if in range, ATTACK!
					if (dist < 100){
						telegraph(); //telegraph our attack first so players have a chance to dodge
						aiState = SKELETON_AI_NORMAL;

					}
					else
					{
						//otherwise move up to the player/target
						aiState = SKELETON_AI_CHASE;
						moving = true;
						changeAnimation(SKELETON_STATE_MOVE, state != SKELETON_STATE_MOVE);
					}
				}
				else
				{
					//no targets, go idle
					moving = false;
					aiState = SKELETON_AI_NORMAL;
					changeAnimation(SKELETON_STATE_IDLE, true);
				}
			}
			
		}
	}
	//if chasing a target, then do that
	if (aiState == SKELETON_AI_CHASE && hp > 0 && active){
		//get the angle between me and the target
		angle = Entity::angleBetweenTwoEntities(this, target);
		//move that way
		move(angle);
	}
}
void Skeleton::telegraph(){
	moving = false;
	frameTimer = 0;
	changeAnimation(SKELETON_STATE_TELEGRAPH, true);
}
void Skeleton::attack(){
	moving = false;
	frameTimer = 0;
	slideAmount = 100;
	slideAngle = angle;
	changeAnimation(SKELETON_STATE_ATTACK, true);
}
void Skeleton::die(){
	moving = false;
	state = SKELETON_STATE_DEAD;
	changeAnimation(state, true);
	//TODO add dying sound to enemy!

	//add to our score count
	Skeleton::killCnt++;
}
void Skeleton::findNearestTarget(){
	target = NULL;
	//find closest target
	for (auto entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++){
		if ((*entity)->type == "hero" && (*entity)->active){
			//if found first hero in list, just set them as the target for now
			if (target == NULL){
				target = (LivingEntity*)(*entity); //if cant cast to LivingEntity, throw casting exception
			}
			else
			{
				//otherwise, is this other hero closer then the previous target
				if (Entity::distanceBetweenTwoEntities(this, (*entity)) < Entity::distanceBetweenTwoEntities(this, target))
				{
					target = (LivingEntity*)(*entity);
				}
			}
		}
	}

}
void Skeleton::changeAnimation(int newState, bool resetFrameToBeginning){
	state = newState;

	if (state == SKELETON_STATE_IDLE){
		if (direction == DIR_DOWN)
			currentAnim = animSet->getAnimation(SKELETON_ANIM_IDLE_DOWN);
		if (direction == DIR_UP)
			currentAnim = animSet->getAnimation(SKELETON_ANIM_IDLE_UP);
		if (direction == DIR_LEFT)
			currentAnim = animSet->getAnimation(SKELETON_ANIM_IDLE_LEFT);
		if (direction == DIR_RIGHT)
			currentAnim = animSet->getAnimation(SKELETON_ANIM_IDLE_RIGHT);
	}
	else if (state == SKELETON_STATE_MOVE){
		if (direction == DIR_DOWN)
			currentAnim = animSet->getAnimation(SKELETON_ANIM_DOWN);
		if (direction == DIR_UP)
			currentAnim = animSet->getAnimation(SKELETON_ANIM_UP);
		if (direction == DIR_LEFT)
			currentAnim = animSet->getAnimation(SKELETON_ANIM_LEFT);
		if (direction == DIR_RIGHT)
			currentAnim = animSet->getAnimation(SKELETON_ANIM_RIGHT);
	}
	else if (state == SKELETON_STATE_ATTACK){
		if (direction == DIR_DOWN)
			currentAnim = animSet->getAnimation(SKELETON_ANIM_ATTACK_DOWN);
		if (direction == DIR_UP)
			currentAnim = animSet->getAnimation(SKELETON_ANIM_ATTACK_UP);
		if (direction == DIR_LEFT)
			currentAnim = animSet->getAnimation(SKELETON_ANIM_ATTACK_LEFT);
		if (direction == DIR_RIGHT)
			currentAnim = animSet->getAnimation(SKELETON_ANIM_ATTACK_RIGHT);
	}
	else if (state == SKELETON_STATE_TELEGRAPH){
		if (direction == DIR_DOWN)
			currentAnim = animSet->getAnimation(SKELETON_ANIM_TELEGRAPH_DOWN);
		if (direction == DIR_UP)
			currentAnim = animSet->getAnimation(SKELETON_ANIM_TELEGRAPH_UP);
		if (direction == DIR_LEFT)
			currentAnim = animSet->getAnimation(SKELETON_ANIM_TELEGRAPH_LEFT);
		if (direction == DIR_RIGHT)
			currentAnim = animSet->getAnimation(SKELETON_ANIM_TELEGRAPH_RIGHT);
	}
	else if (state == SKELETON_STATE_DEAD){
		currentAnim = animSet->getAnimation(SKELETON_ANIM_DIE);
	}

	if (resetFrameToBeginning)
		currentFrame = currentAnim->getFrame(0);
	else
		currentFrame = currentAnim->getFrame(currentFrame->frameNumber);
}
void Skeleton::updateAnimation(){
	if (currentFrame == NULL || currentAnim == NULL)
		return;

	//if we're in moveState but not actually going anywhere then
	if (state == SKELETON_STATE_MOVE && !moving){
		changeAnimation(SKELETON_STATE_IDLE, true);
	}
	//if we're idle but we're actually moving, then
	if (state != SKELETON_STATE_MOVE && moving){
		changeAnimation(SKELETON_STATE_MOVE, true);
	}

	frameTimer += TimeState::timeState.dT;
	//time to change frames
	if (frameTimer >= currentFrame->duration){
		//if at the end of the animation
		if (currentFrame->frameNumber == currentAnim->getEndFrameNumber()){
			if (state == SKELETON_STATE_TELEGRAPH){
				//done telegraphing, now attack
				attack();
			}
			else if (state == SKELETON_STATE_ATTACK){
				changeAnimation(SKELETON_STATE_MOVE, true);
			}
			else if (state == SKELETON_STATE_DEAD){
				frameTimer = 0;
				//if some how alive again, then change state back to moving
				if (hp > 0)
					changeAnimation(SKELETON_STATE_MOVE, true);
				else
					active = false; 
			}
			else
			{
				//loop animation
				currentFrame = currentAnim->getFrame(0);
			}
		}
		else{
			//otherwise just move to the next frame
			currentFrame = currentAnim->getNextFrame(currentFrame);
		}
		frameTimer = 0;
	}
}
void Skeleton::updateDamages(){
	if (active && hp > 0 && invincibleTimer <= 0){
		for (auto entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++){
			if ((*entity)->active && (*entity)->type == "hero"){
				//reference as LivingEntity, so we can access hitBox/damage
				LivingEntity* enemy = (LivingEntity*)(*entity);
				//if enemy hits me, thennnnnnnnn
				if (enemy->damage > 0 && Entity::checkCollision(collisionBox, enemy->hitBox)){
					hp -= enemy->damage;

					if (hp > 0){
						//TODO play hurt sound
						invincibleTimer = 0.1;
					}
					//angle from other entity, towards this entity (get thrown backwards)
					slideAngle = Entity::angleBetweenTwoEntities((*entity), this);
					slideAmount = 300;
				}
			}
		}
	}
}