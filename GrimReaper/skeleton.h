#ifndef SKELETON_H
#define SKELETON_H

#include <cstdlib>
#include "living_entity.h"

class Skeleton : public LivingEntity {
public:
	static const string SKELETON_ANIM_UP;
	static const string SKELETON_ANIM_DOWN;
	static const string SKELETON_ANIM_LEFT;
	static const string SKELETON_ANIM_RIGHT;
	static const string SKELETON_ANIM_IDLE_UP;
	static const string SKELETON_ANIM_IDLE_DOWN;
	static const string SKELETON_ANIM_IDLE_LEFT;
	static const string SKELETON_ANIM_IDLE_RIGHT;
	static const string SKELETON_ANIM_ATTACK_UP;
	static const string SKELETON_ANIM_ATTACK_DOWN;
	static const string SKELETON_ANIM_ATTACK_LEFT;
	static const string SKELETON_ANIM_ATTACK_RIGHT;
	static const string SKELETON_ANIM_TELEGRAPH_UP;
	static const string SKELETON_ANIM_TELEGRAPH_DOWN;
	static const string SKELETON_ANIM_TELEGRAPH_LEFT;
	static const string SKELETON_ANIM_TELEGRAPH_RIGHT;
	static const string SKELETON_ANIM_DIE;

	static const int SKELETON_STATE_IDLE;
	static const int SKELETON_STATE_MOVE;
	static const int SKELETON_STATE_ATTACK;
	static const int SKELETON_STATE_TELEGRAPH;
	static const int SKELETON_STATE_DEAD;

	static const int SKELETON_AI_NORMAL;
	static const int SKELETON_AI_CHASE;

	static int killCnt;

	float thinkTimer = 0;

	LivingEntity* target = NULL;

	int aiState = SKELETON_AI_NORMAL;

	Skeleton(AnimationSet *animSet);
	void update();
	void think();
	void telegraph();
	void attack();
	void die();
	void findNearestTarget();
	void changeAnimation(int newState, bool resetFrameToBeginning);
	void updateAnimation();
	void updateDamages();
};

#endif