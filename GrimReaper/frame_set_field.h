#ifndef FRAMESETFIELD_H
#define FRAMESETFIELD_H

#include "frame_set.h"

class FrameSetField : public FrameSet{
public:
	std::list<std::string> data;

	FrameSetField(FrameSetType type){
		this->type = type;
	}

	int frameSetCount(){
		return data.size();
	}

	void addToSet(std::string str){
		if (str[0] == ' '){
			str.erase(0, 1);
		}

		data.push_back(str);
	}

	void draw(){

	}
};


#endif