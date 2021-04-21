#ifndef TIMER_H
#define TIMER_H

#include "sdl/SDL.h"
#include <iostream>
#include <string>
#include "core/Logger.h"

constexpr float MILLI_TO_SEC = 1000.0f;

class Timer
{
private:
	static unsigned int currentTicks;
	static unsigned int prevTicks;

	static float timeScale;
public:

	Timer() = delete;
	Timer(const Timer&) = delete;
	Timer(Timer&&) = delete;
	Timer& operator=(const Timer&) = delete;
	Timer& operator=(Timer&&) = delete;
	~Timer() = delete;

	inline static void SetTimeScale(const float ts) { timeScale = ts; EngineLogger::Info("Time scale changed to: " + std::to_string(timeScale), "Timer.h", __LINE__); }
	static float GetDeltaTime();
	static float GetScaledDeltaTime();
	static void UpdateTimer();
	static unsigned int SleepTime(const unsigned int fps);

	static float GetCurrentTickMilli();
	static float GetCurrentTickSec();

};

#endif