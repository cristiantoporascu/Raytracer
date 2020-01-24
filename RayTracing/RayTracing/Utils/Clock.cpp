#include "Clock.h"

void Clock::Start()
{
	this->m_startTime = SDL_GetTicks();
	this->m_timerRunning = true;
}

void Clock::Stop()
{
	this->m_timerRunning = false;
}

Uint32 Clock::getTimeMilliseconds() const
{
	if (this->m_timerRunning)
		return SDL_GetTicks() - this->m_startTime;
	else
		return 0;
}