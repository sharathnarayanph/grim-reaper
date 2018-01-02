#include "time_state.h"

const int TimeState::ST_PLAY = 0, TimeState::ST_PAUSE = 1;

TimeState::TimeState(){
	dT = 0;
	lastUpdate = 0;
	stTime = ST_PLAY;
}
void TimeState::updateTime(){
	if (stTime == ST_PAUSE){
		dT = 0;
	}
	else{
		Uint32 timeDiff = SDL_GetTicks() - lastUpdate;
		dT = timeDiff / 1000.0;
	}
	lastUpdate = SDL_GetTicks();
}
void TimeState::pause(){
	stTime = ST_PAUSE;
}
void TimeState::resume(){
	stTime = ST_PLAY;
}
void TimeState::reset(){
	dT = 0;
	lastUpdate = SDL_GetTicks();
}

TimeState TimeState::timeState;