#ifndef ANIMATION
#define ANIMATION

#include "frame.h"

class Animation {
public:
	std::string name;
	std::list<Frame> frames; 

	Animation(string name = "");

	int getNextFrameNumber(int frameNumber);
	Frame *getNextFrame(Frame* frame);
	int getEndFrameNumber(); 
	Frame* getFrame(int frameNumber); 
};

#endif