#ifndef GLOBALS
#define GLOBALS

#include <string>
#include <iostream>
#include <SDL.h>
#include "util.h"

class Globals{
public:

	static const float PI;
	static bool debugging;
	static int ScreenWidth, ScreenHeight, ScreenScale;
	static SDL_Renderer* renderer;

	static std::string clipOffDataHeader(std::string data);
};

#endif