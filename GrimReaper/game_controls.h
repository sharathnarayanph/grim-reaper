#ifndef KEYBOARDINPUT_H
#define KEYBOARDINPUT_H

#include "hero.h"

class GameControls {
public:
	Hero* hero; 
	SDL_Scancode UP, DOWN, LEFT, RIGHT;
	SDL_Scancode SLASH, DASH;

	GameControls();
	void update(SDL_Event* e);
};

#endif