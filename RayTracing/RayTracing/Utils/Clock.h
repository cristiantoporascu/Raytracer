#pragma once
#include "SDL2-2.0.4/include/SDL.h"

class Clock
{
private:
	Uint32 m_startTime;
	bool m_timerRunning = false;

public:
	void Start();
	void Stop();

	Uint32 getTimeMilliseconds() const;
	inline bool IsRunning() const { return this->m_timerRunning; }
};