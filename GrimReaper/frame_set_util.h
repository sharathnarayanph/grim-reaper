#ifndef FRAMESETUTIL_H
#define FRAMESETUTIL_H

#include "globals.h"
#include "frame_set.h"
#include "frame_set_rect.h"
#include "frame_set_pos.h"
#include "frame_set_val.h"
#include "frame_set_field.h"

class FrameSetUtil
{
public:

	static const bool savedFieldSet;

	static FrameSet* buildSet(FrameSetType dt);

	static void buildSets(std::list<FrameSetType> types, std::list<FrameSet*> &sets);

	//Add fields to frame set
	static FrameSet* addFrameFieldToSet(std::string name, std::list<FrameSet*> &sets);

	static void loadSets(std::ifstream &file, std::list<FrameSet*> &sets);

	static FrameSet* findSetByName(std::string str, std::list<FrameSet*> &sets);
};

#endif