#include "frame_set_util.h"

const bool FrameSetUtil::savedFieldSet = false;

FrameSet* FrameSetUtil::buildSet(FrameSetType dataType) {
	FrameSet* frameSet = NULL;

	if (dataType.fsType == FrameSetType::FSTYPE_VAL) {
		frameSet = new FrameSetVal(dataType);
	}
	else if (dataType.fsType == FrameSetType::FSTYPE_POS) {
		frameSet = new FrameSetPos(dataType);
	}
	else if (dataType.fsType == FrameSetType::FSTYPE_RECT) {
		frameSet = new FrameSetRect(dataType);
	}
	else {
		frameSet = new FrameSetField(dataType);
	}

	return frameSet;
}

void FrameSetUtil::buildSets(std::list<FrameSetType> type, std::list<FrameSet*> &sets) {
	for (std::list<FrameSetType>::iterator fst = type.begin(); fst != type.end(); fst++) {
		FrameSet *frameSet = buildSet((*fst));
		sets.push_back(frameSet);
	}
}

FrameSet* FrameSetUtil::addFrameFieldToSet(std::string name, std::list<FrameSet*> &sets) {
	FrameSetType fst;
	fst.fsType = FrameSetType::FSTYPE_FIELD;
	fst.setName = name;
	fst.isSingle = false;
	FrameSet *frameSet = new FrameSetField(fst);
	sets.push_back(frameSet);
	return frameSet;
}

void FrameSetUtil::loadSets(std::ifstream &file, std::list<FrameSet*> &sets) {

	while (!file.eof()) {
		int positionBeforeRead = file.tellg();
		std::string line;
		getline(file, line);
		if (line.empty() || line == "")
			break;
		if (savedFieldSet) {
			int pos = line.find(":", 0);
			if (pos == -1) {
				file.seekg(positionBeforeRead);
				break;
			}
			FrameSet *frameSet = findSetByName(line.substr(0, pos), sets);
			if (frameSet == NULL) {
				frameSet = addFrameFieldToSet(line.substr(0, pos), sets);
			}

			std::string numStr = line.substr(pos + 1, line.length() - pos + 2);
			std::stringstream ss;
			ss << numStr << std::endl;
			int num;
			ss >> num;

			for (int i = 0; i < num; i++) {
				if (!file.good())
					break;

				getline(file, line);

				frameSet->addToSet(line);
			}
				 
		}
		else {
			int pos = line.find(":", 0);
			if (pos == -1){
				file.seekg(positionBeforeRead);
				break;
			}
			std::cout << "sub: " << line.substr(0, pos) << std::endl;
			FrameSet *frameSet = findSetByName(line.substr(0, pos), sets);
			if (frameSet == NULL) 
				frameSet = addFrameFieldToSet(line.substr(0, pos), sets);

			//line = Globals::clipOffDataHeader(line);
			frameSet->addToSet(line);		
		}
	}
}

FrameSet* FrameSetUtil::findSetByName(std::string str, std::list<FrameSet*> &sets){
	for (std::list<FrameSet*>::iterator frameSet = sets.begin(); frameSet != sets.end(); frameSet++) {
		if (str == (*frameSet)->type.setName)
			return (*frameSet);
	}

	return NULL;
}