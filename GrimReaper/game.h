#ifndef GAME_H
#define GAME_H

#include "globals.h"
#include "hero.h"
#include "arena.h"
#include "game_controls.h"
#include "drawing_functions.h"

class Game {
public:
	Game();

	AnimationSet* heroAnimSet;
	AnimationSet* wallAnimSet;
	SDL_Texture* backgroundImage;
	Hero *hero;
	GameControls heroInput;
	list<Entity*> walls;

	void update();
	void draw();

	~Game();
};


#endif