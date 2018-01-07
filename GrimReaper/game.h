#ifndef GAME_H
#define GAME_H

#include "globals.h"
#include "hero.h"
#include "skeleton.h"
#include "arena.h"
#include "game_controls.h"
#include "drawing_functions.h"
#include "sound_manager.h"

class Game {
public:
	Game();

	Mix_Music* song;
	AnimationSet* heroAnimSet;
	AnimationSet* skeletonAnimSet;
	AnimationSet* wallAnimSet;
	SDL_Texture* backgroundImage;
	SDL_Texture* splashImage;
	SDL_Texture* overlayImage;
	SDL_Texture* scoreTexture = NULL;
	Hero *hero;
	GameControls heroInput;
	list<Entity*> walls;
	list<Entity*> enemies;

	bool splashShowing;
	float overlayTimer;

	void update();
	void draw();

	~Game();
};


#endif