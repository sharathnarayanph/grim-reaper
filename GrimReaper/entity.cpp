#include "entity.h"

const int Entity::DIR_UP = 0, Entity::DIR_DOWN = 1, Entity::DIR_LEFT = 2, Entity::DIR_RIGHT= 3, Entity::DIR_NONE = -1;
std::list<Entity*> Entity::entities;

void Entity::update() { 

}

void Entity::draw() {
	if (currentFrame != NULL && active) {
		currentFrame->Draw(animSet->spriteSheet, x, y);
	}
	//Draw collision box
	if (solid && Globals::debugging) {
		SDL_SetRenderDrawColor(Globals::renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(Globals::renderer, &collisionBox);
	}
}

void Entity::move(float angle) {
	moving = true;
	moveSpeed = moveSpeedMax;
	this->angle = angle;

	int newDirection = angleToDirection(angle);
	//If direction changed, update current animation
	if (direction != newDirection){
		direction = newDirection;
		changeAnimation(state, false);
	}
}

void Entity::updateMovement() {
	updateCollisionBox();
	lastCollisionBox = collisionBox;

	totalXMove = 0;
	totalYMove = 0;

	if (moveSpeed > 0){
		//Works out move distance using speed, dt(time since last loop) and multiplies by lerp
		float moveDist = moveSpeed*(TimeState::timeState.dT)*moveLerp;
		if (moveDist > 0) {
			//xmove = distance * cos(angle in radians)
			float xMove = moveDist*(cos(angle*Globals::PI / 180));
			//ymove = distance * sin(angle in radians)
			float yMove = moveDist*(sin(angle*Globals::PI / 180));

			x += xMove;
			y += yMove;

			totalXMove = xMove;
			totalYMove = yMove;

			if (!moving)
				moveSpeed -= moveDist;
		}
	}

	if (slideAmount > 0){
		float slideDist = slideAmount*TimeState::timeState.dT*moveLerp;
		if (slideDist > 0) {
			float xMove = slideDist*(cos(slideAngle*Globals::PI / 180));
			float yMove = slideDist*(sin(slideAngle*Globals::PI / 180));

			x += xMove;
			y += yMove;

			totalXMove += xMove;
			totalYMove += yMove;
			slideAmount -= slideDist;
		}
		else
			slideAmount = 0;
	}
	updateCollisionBox();
	//Check collision with entities
	SDL_UnionRect(&collisionBox, &lastCollisionBox, &collisionBox);
}

void Entity::updateCollisionBox() {
	collisionBox.x = x - collisionBox.w / 2;
	collisionBox.y = y + collisionBoxYOffset;
	collisionBox.w = collisionBoxW;
	collisionBox.h = collisionBoxH;
}

void Entity::updateCollisions() {
	if (active && collideWithSolids && (moveSpeed > 0 || slideAmount > 0)) {
		//List of potential collisions
		list<Entity*> collisions;

		for (auto entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++) {
			//If the main entity collides with the current entity, add to list
			if ((*entity)->active 
				&& (*entity)->type != this->type 
				&& (*entity)->solid 
				&& Entity::checkCollision(collisionBox, (*entity)->collisionBox)) {

				collisions.push_back(*entity);

			}
		}
		//If collision available, perform collision resolution
		if (collisions.size() > 0){
			updateCollisionBox();

			//Sample distance between collision entities
			float boxTravelSize = 0;
			if (collisionBox.w < collisionBox.h)
				boxTravelSize = collisionBox.w / 4;
			else
				boxTravelSize = collisionBox.h / 4;

			//Use sampleBox to check for collisions from start point to end point
			SDL_Rect sampleBox = lastCollisionBox;
			float movementAngle = Entity::angleBetweenTwoRects(lastCollisionBox, collisionBox);

			bool foundCollision = false; 
			while (!foundCollision){
				//Check samplebox for collisions where it is now
				SDL_Rect intersection;
				for (auto entity = collisions.begin(); entity != collisions.end(); entity++){
					if (SDL_IntersectRect(&sampleBox, &(*entity)->collisionBox, &intersection))
					{
						foundCollision = true;
						moveSpeed = 0;
						moving = false;
						slideAngle = angleBetweenTwoEntities((*entity), this);

						//Collision resolution with currently colliding entity
						//Resolve collision based on the lowest value of height or width
						if (intersection.w < intersection.h) {
							if (lastCollisionBox.x + lastCollisionBox.w / 2 < (*entity)->collisionBox.x + (*entity)->collisionBox.w / 2)
								sampleBox.x -= intersection.w; 
							else
								sampleBox.x += intersection.w; 
						}
						else {
							if (lastCollisionBox.y + lastCollisionBox.h / 2 < (*entity)->collisionBox.y + (*entity)->collisionBox.h / 2)
								sampleBox.y -= intersection.h; 
							else
								sampleBox.y += intersection.h; 
						}
					}
				}

				//If collision and sample box at same place, exit loop
				if (foundCollision || (sampleBox.x == collisionBox.x && sampleBox.y == collisionBox.y))
					break;

				//Move sample box up to check the next spot
				if (distanceBetweenTwoRects(sampleBox, collisionBox) > boxTravelSize) {
					float xMove = boxTravelSize * (cos(movementAngle*Globals::PI / 180));
					float yMove = boxTravelSize * (sin(movementAngle * Globals::PI / 180));

					sampleBox.x += xMove;
					sampleBox.y += yMove;
				}
				else {
					sampleBox = collisionBox;
				}
			}
			
			if (foundCollision) {
				slideAmount = slideAmount / 2;
				x = sampleBox.x + sampleBox.w / 2;
				y = sampleBox.y - collisionBoxYOffset;
			}

			updateCollisionBox();
		}
	}
}


float Entity::distanceBetweenTwoRects(SDL_Rect &r1, SDL_Rect &r2) {
	SDL_Point e1, e2;
	e1.x = r1.x + r1.w / 2;
	e1.y = r1.y + r1.h / 2;

	e2.x = r2.x + r2.w / 2;
	e2.y = r2.y + r2.h / 2;

	float d = abs(sqrt(pow(e2.x - e1.x, 2) + pow(e2.y - e1.y, 2)));
	return d;
}

float Entity::distanceBetweenTwoEntities(Entity *e1, Entity *e2) {
	float d = abs(sqrt(pow(e2->x - e1->x, 2) + pow(e2->y - e1->y, 2)));
	return d;
}

float Entity::angleBetweenTwoEntities(Entity *e1, Entity *e2) {
	float dx, dy;
	float x1 = e1->x, y1 = e1->y, x2 = e2->x, y2 = e2->y;
	
	dx = x2 - x1;
	dy = y2 - y1;

	return atan2(dy, dx) * 180 / Globals::PI;
}

bool Entity::checkCollision(SDL_Rect cbox1, SDL_Rect cbox2) {
	SDL_Rect intersection;
	if (SDL_IntersectRect(&cbox1, &cbox2, &intersection))
		return true;

	return false;
}

int Entity::angleToDirection(float angle) {
	if ((angle >= 0 && angle <= 45) || angle >= 315 && angle <= 360)
		return DIR_RIGHT;
	else if (angle >= 45 && angle <= 135)
		return DIR_DOWN;
	else if (angle >= 135 && angle <= 225)
		return DIR_LEFT;
	else
		return DIR_UP;
}
float Entity::angleBetweenTwoPoints(float cx1, float cy1, float cx2, float cy2) {
	float dx = cx2 - cx1;
	float dy = cy2 - cy1;

	return atan2(dy, dx) * 180 / Globals::PI;
}
float Entity::angleBetweenTwoRects(SDL_Rect &r1, SDL_Rect &r2) {
	float cx1 = r1.x + (r1.w / 2);
	float cy1 = r1.y + (r1.h / 2);

	float cx2 = r2.x + (r2.w / 2);
	float cy2 = r2.y + (r2.h / 2);

	return angleBetweenTwoPoints(cx1, cy1, cx2, cy2);
}

bool Entity::EntityCompare(const Entity* const &a, const Entity * const &b) {
	if (a != 0 && b != 0)
		return (a->y < b->y);
	return false;
}

void Entity::removeInactiveEntitiesFromList(list<Entity*> *entityList, bool deleteEntities) {
	for (auto entity = entityList->begin(); entity != entityList->end();) {
		//If entity is not active
		if (!(*entity)->active) {
			if (deleteEntities)
				delete (*entity);
			entity = entityList->erase(entity);
		}
		else
			entity++;
	}
}

void Entity::removeAllFromList(list<Entity*> *entityList, bool deleteEntities) {
	for (auto entity = entityList->begin(); entity != entityList->end();) {
		if (deleteEntities)
			delete (*entity);
		entity = entityList->erase(entity);
	}
}