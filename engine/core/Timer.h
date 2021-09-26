#ifndef TIMER_H
#define TIMER_H


#include <iostream>
#include <string>
#include "core/Logger.h"
#include <vector>


constexpr float MILLI_TO_SEC = 1000.0f;

class Timer
{
private:
	static unsigned int initialTick;
	static unsigned int currentTicks;
	static unsigned int prevTicks;
	static unsigned int elapsedTicks;
	static std::vector<unsigned int> recentTicks;

	static float averageTickrate;
	static float timeScale;


	

public:
	Timer();
	~Timer();
	
	Timer(const Timer&) = delete;
	Timer(Timer&&) = delete;
	Timer& operator=(const Timer&) = delete;
	Timer& operator=(Timer&&) = delete;
	
	static float GetDeltaTime();
	static float GetScaledDeltaTime();
	static float GetAverageFramerate();
	static float GetCurrentTickMilli();
	static float GetCurrentTickSec();
	static void UpdateTimer();
	static void Reset();
	static unsigned int SleepTime(const unsigned int fps);

	inline static void SetTimeScale(const float ts) { timeScale = ts; EngineLogger::Info("Time scale changed to: " + std::to_string(timeScale), "Timer.h", __LINE__); }

};

#endif