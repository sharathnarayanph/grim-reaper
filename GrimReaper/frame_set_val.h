#ifndef FRAMESETVAL_H
#define FRAMESETVAL_H

#include "frame_set.h"

class FrameSetVal : public FrameSet{
public:
	std::list<float> data;

	FrameSetVal(FrameSetType type){
		this->type = type;
	}

	int frameSetCount(){
		return data.size();
	}

	void addToSet(std::string str){
		std::stringstream ss;
		ss << str;
		float r;
		ss >>r;

		data.push_back(r);
	}

	void draw(){

	}
};


#endif