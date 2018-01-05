#ifndef FRAMESET_H
#define FRAMESET_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <SDL.h>
#include "frame_set_type.h"

class FrameSet {
public:
	FrameSetType type;

	FrameSet() {

	}

	FrameSet(FrameSetType type) {
		this->type = type;
	}

	virtual int frameSetCount() = 0;

	virtual void addToSet(std::string str) = 0;

	virtual void draw() = 0;
};

#endif