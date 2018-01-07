#ifndef GAME_H
#define GAME_H

#include "globals.h"
#include "hero.h"
#include "skeleton.h"
#include "arena.h"
#include "game_controls.h"
#include "drawing_functions.h"

class Game {
public:
	Game();

	AnimationSet* heroAnimSet;
	AnimationSet* skeletonAnimSet;
	AnimationSet* wallAnimSet;
	SDL_Texture* backgroundImage;
	Hero *hero;
	GameControls heroInput;
	list<Entity*> walls;
	list<Entity*> enemies;

	void update();
	void draw();

	~Game();
};


#endif