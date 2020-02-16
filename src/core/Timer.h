#ifndef TIMER_H
#define TIMER_H

#include "sdl/SDL.h"
#include <iostream>

class Timer
{
private:
	static unsigned int currentTicks;
	static unsigned int prevTicks;

	static float timeScale;
public:

	Timer();
	~Timer();

	inline static void SetTimeScale(float ts) { timeScale = ts; std::cout << "Time scale changed to: " << timeScale << std::endl; }
	static float GetDeltaTime();
	static float GetScaledDeltaTime();
	static void UpdateTimer();
	static unsigned int SleepTime(const unsigned int fps);

};

#endif