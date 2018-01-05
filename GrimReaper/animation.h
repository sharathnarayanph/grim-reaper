#ifndef ANIMATION_H
#define ANIMATION_H

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

	void loadAnimation(std::ifstream &file, std::list<FrameSetType> &types);
};

#endif