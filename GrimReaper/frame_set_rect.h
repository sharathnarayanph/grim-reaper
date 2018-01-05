#ifndef FRAMESETRECT_H
#define FRAMESETRECT_H

#include "frame_set.h"

class FrameSetRect : public FrameSet{
public:
	std::list<SDL_Rect> data;

	FrameSetRect(FrameSetType type){
		this->type = type;
	}

	int frameSetCount(){
		return data.size();
	}
	
	void addToSet(std::string str){
		std::stringstream ss;
		ss << str;
		SDL_Rect box;
		ss >> box.x >> box.y >> box.w >> box.h;

		data.push_back(box);
	}

	void draw(){

	}

};

#endif