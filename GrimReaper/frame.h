#ifndef FRAME_H
#define FRAME_H

#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include "SDL.h"
#include "drawing_functions.h"
#include "globals.h"
#include "frame_set_util.h"

class Frame {
public:
	int frameNumber;
	SDL_Rect clip; 
	float duration;
	SDL_Point offSet;

	std::list<FrameSet*> frameData;

	void Draw(SDL_Texture* spriteSheet, float x, float y);
};

#endif