#ifndef TIMESTATE_H
#define TIMESTATE_H

#include <iostream>
#include "SDL.h"

class TimeState{
public:

	static const int ST_PLAY, ST_PAUSE;

	int stTime;
	Uint32 lastUpdate; 
	float dT; //Time since last render

	TimeState();
	void updateTime();
	void pause();
	void resume();
	void reset();

	static TimeState timeState;
};


#endif