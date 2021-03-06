#include "animation.h"

Animation::Animation(string name) {
	this->name = name;
}

int Animation::getNextFrameNumber(int frameNumber) {
	if (frameNumber + 1 < frames.size())
		return frameNumber + 1;

    return 0;
}

Frame* Animation::getNextFrame(Frame* frame) {
	return getFrame(getNextFrameNumber(frame->frameNumber));
}

int Animation::getEndFrameNumber() {
	return frames.size() - 1;
}

Frame* Animation::getFrame(int frameNumber) {
	if (frames.size() == 0)
		return NULL;

	list<Frame>::iterator itr = frames.begin(); 

	for (int counter = 0; counter < frameNumber && counter < frames.size() - 1; counter++){
		itr++; 
	}

	Frame *frame = &(*itr); 

	return frame;
}

void Animation::loadAnimation(std::ifstream &file, list<FrameSetType> &types) {
	getline(file, name);
	std::string buffer;
	getline(file, buffer);
	std::stringstream ss;
	buffer = Globals::clipOffDataHeader(buffer);
	ss << buffer;
	int numberOfFrames;
	ss >> numberOfFrames;

	for (int i = 0; i < numberOfFrames; i++) {
		Frame newFrame;
		newFrame.loadFrame(file, types);
		frames.push_back(newFrame);
	}
}