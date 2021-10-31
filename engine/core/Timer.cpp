#include "Timer.h"

unsigned int Timer::currentTicks = 0;
unsigned int Timer::prevTicks = 0;
float Timer::timeScale = 1.0f;


float Timer::GetDeltaTime()
{
	return (static_cast<float>(currentTicks - prevTicks) / MILLI_TO_SEC);
}

float Timer::GetScaledDeltaTime()
{
	return (static_cast<float>(currentTicks - prevTicks) / MILLI_TO_SEC)* timeScale;
}

void Timer::UpdateTimer()
{
	prevTicks = currentTicks;
	currentTicks = SDL_GetTicks();
}

unsigned int Timer::SleepTime(const unsigned int fps)
{

	const unsigned int milliSecsPerFrame = static_cast<unsigned int>(MILLI_TO_SEC) / fps;

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

float Timer::GetCurrentTickMilli()
{
	return static_cast<float>(currentTicks);
}

float Timer::GetCurrentTickSec()
{
	return static_cast<float>(currentTicks) / MILLI_TO_SEC;
}
