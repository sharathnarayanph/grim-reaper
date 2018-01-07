#include "game_controls.h"

GameControls::GameControls() {
	UP = SDL_SCANCODE_UP;
	DOWN = SDL_SCANCODE_DOWN;
	LEFT = SDL_SCANCODE_LEFT;
	RIGHT = SDL_SCANCODE_RIGHT;
	SLASH = SDL_SCANCODE_Z;
	DASH = SDL_SCANCODE_X;

	//int test = 82;
	//UP = (SDL_Scancode)test;
}
void GameControls::update(SDL_Event* e) {

	if (e->type == SDL_KEYDOWN){
		if (e->key.keysym.scancode == DASH){
			hero->dash();
		}
		if (e->key.keysym.scancode == SLASH){
			hero->slash();
		}
	}

	const Uint8 *keystates = SDL_GetKeyboardState(NULL);
	//If hero not able to move, halt
	if ((hero->state != Hero::HERO_STATE_MOVE && hero->state != Hero::HERO_STATE_IDLE) || (!keystates[UP] && !keystates[DOWN] && !keystates[LEFT] && !keystates[RIGHT]))
		hero->moving = false;

	else {
		if (keystates[UP]) {
			if (keystates[RIGHT])
				hero->move(270 + 45);
			else if (keystates[LEFT])
				hero->move(270 - 45);
			else
				hero->move(270);
		}

		if (keystates[DOWN]) {
			if (keystates[RIGHT])
				hero->move(90 - 45);
			else if (keystates[LEFT])
				hero->move(90 + 45);
			else
				hero->move(90);
		}

		if (!keystates[UP] && !keystates[DOWN] && !keystates[RIGHT] && keystates[LEFT])
			hero->move(180);

		if (!keystates[UP] && !keystates[DOWN] && keystates[RIGHT] && !keystates[LEFT])
			hero->move(0);
	}
}