#include "Timer.h"

namespace soku {
	Timer::Timer()
		:m_baseTime(0),
		m_pausedTime(0),
		m_stopTime(0),
		m_prevTime(0),
		m_currTime(0),
		m_secondsPerCount(0),
		mDeltaTime(-1.f),
		m_stoped(false)
	{
		__int64 countPerSecond;
		QueryPerformanceFrequency((LARGE_INTEGER*)&countPerSecond);
		m_secondsPerCount = 1.0 / (double)countPerSecond;
	}
	double Timer::GetTickSeconds() const
	{
		return m_secondsPerCount;
	}
}