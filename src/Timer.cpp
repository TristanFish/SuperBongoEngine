#include "Timer.h"

unsigned int Timer::currentTicks = 0;
unsigned int Timer::prevTicks = 0;
float Timer::timeScale = 1.0f;

Timer::Timer()
{
	std::cout << "Timer constructor accessed" << std::endl;
}

Timer::~Timer()
{
}

float Timer::GetDeltaTime()
{
	return (static_cast<float>(currentTicks - prevTicks) / 1000.0f);
}

float Timer::GetScaledDeltaTime()
{
	return (static_cast<float>(currentTicks - prevTicks) / 1000.0f)* timeScale;
}

void Timer::UpdateTimer()
{
	prevTicks = currentTicks;
	currentTicks = SDL_GetTicks();
}

unsigned int Timer::SleepTime(const unsigned int fps)
{

	unsigned int milliSecsPerFrame = 1000 / fps;
	unsigned int sleepTime = milliSecsPerFrame - SDL_GetTicks();

	if (sleepTime > milliSecsPerFrame)
	{
		return milliSecsPerFrame;
	}

	return sleepTime;

}
