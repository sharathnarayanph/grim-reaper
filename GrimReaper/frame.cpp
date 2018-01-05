#include "frame.h"

void Frame::Draw(SDL_Texture* spriteSheet, float x, float y) {
	SDL_Rect dest; 
	dest.x = x - offSet.x;
	dest.y = y - offSet.y;
	dest.w = clip.w;
	dest.h = clip.h;

	renderTexture(spriteSheet, Globals::renderer, dest, &clip);
}

void Frame::loadFrame(ifstream &file, list<FrameSetType> &types){

	FrameSetUtil::buildSets(types, frameData);


	std::string buffer;
	getline(file, buffer);
	getline(file, buffer);
	std::stringstream ss; 
	buffer = Globals::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> clip.x >> clip.y >> clip.w >> clip.h;

	//Offset value for frames
	getline(file, buffer);
	ss.clear();
	buffer = Globals::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> offSet.x >> offSet.y;

	//Duration for each frame
	getline(file, buffer);
	ss.clear();
	buffer = Globals::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> duration;

	//Index
	getline(file, buffer);
	ss.clear();
	buffer = Globals::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> frameNumber;

	FrameSetUtil::loadSets(file, frameData);
}