#ifndef FRAMESETPOS_H
#define FRAMESETPOS_H

#include "frame_set.h"

class FrameSetPos : public FrameSet{
public:
	std::list<SDL_Point> data;

	FrameSetPos(FrameSetType type){
		this->type = type;
	}

	int frameSetCount(){
		return data.size();
	}
	
	void addToSet(std::string str){
		std::stringstream ss;
		ss << str;
		SDL_Point pt;
		ss >> pt.x >> pt.y ;

		data.push_back(pt);
	}

	void draw(){

	}
};

#endif