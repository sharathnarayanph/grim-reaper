#ifndef FRAMESETTYPE_H
#define FRAMESETTYPE_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class FrameSetType{
public:
	std::string setName; 
	int fsType; //Name or Position or Rect
	bool isSingle;

	static const int FSTYPE_FIELD = 0, FSTYPE_POS = 1, FSTYPE_RECT = 2, FSTYPE_VAL = 3;

	static void saveFrameSetType(std::ofstream &file, FrameSetType fsType);
	static FrameSetType loadFrameSetType(std::ifstream &file);
};

#endif