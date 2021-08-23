#pragma once
#include <SDL/SDL.h>
class Timer
{
private:
	Uint32 frameTime;
public:
	Timer();

	void setFrameTime(Uint32 frameTime);
	Uint32 getFrameTime();
};

