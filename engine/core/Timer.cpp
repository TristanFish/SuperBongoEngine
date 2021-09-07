#include "Timer.h"
#include "sdl/SDL.h"

unsigned int Timer::initialTick = 0;
unsigned int Timer::currentTicks = 0;
unsigned int Timer::prevTicks = 0;
unsigned int Timer::elapsedTicks = 0;

std::vector<unsigned int> Timer::recentTicks = std::vector<unsigned int>();

float Timer::averageTickrate = 0.0f;
float Timer::timeScale = 1.0f;

Timer::Timer()	{
	recentTicks.reserve(4);
	Reset();
}

Timer::~Timer()	{
	recentTicks.clear();
}


float Timer::GetDeltaTime()
{
	return (static_cast<float>(currentTicks - initialTick) / MILLI_TO_SEC);
}

float Timer::GetScaledDeltaTime()
{
	return (static_cast<float>(currentTicks - initialTick) / MILLI_TO_SEC)* timeScale;
}

float Timer::GetAverageFramerate()	{
	return 1 / averageTickrate * MILLI_TO_SEC;
}

void Timer::UpdateTimer()	{
	
	prevTicks = currentTicks;
	currentTicks = SDL_GetTicks();
	
	recentTicks.push_back(currentTicks-prevTicks);
	while (recentTicks.size() > 4)	{
		recentTicks.erase(recentTicks.begin());
	}
	if (recentTicks.size() >= 4)
		averageTickrate = ((recentTicks.data()[0] + recentTicks.data()[1] + recentTicks.data()[2] + recentTicks.data()[3]) / 4.0f);
}

unsigned int Timer::SleepTime(const unsigned int fps)
{

	const unsigned int milliSecsPerFrame = MILLI_TO_SEC / fps;

	if (milliSecsPerFrame == 0)
	{
		return 0;
	}

	const unsigned int sleepTime = milliSecsPerFrame - (SDL_GetTicks() - currentTicks);

	if (sleepTime > milliSecsPerFrame)
	{
		return milliSecsPerFrame;
	}

	return sleepTime;

}

void Timer::Reset()	{
	initialTick = SDL_GetTicks();
	prevTicks = 0;
	currentTicks = 0;
	elapsedTicks = 0;
	
}

float Timer::GetCurrentTickMilli()
{
	return static_cast<float>(currentTicks);
}

float Timer::GetCurrentTickSec()
{
	return static_cast<float>(currentTicks) / MILLI_TO_SEC;
}
