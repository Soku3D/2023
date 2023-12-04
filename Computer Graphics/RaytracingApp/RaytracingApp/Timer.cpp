#include "pch.h"

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
	float Timer::GetTime() const
	{
		__int64 count;
		QueryPerformanceCounter((LARGE_INTEGER*)&count);
		return count;
	}
	float Timer::DeltaTime() const
	{
		return (float)mDeltaTime;
	}
	float Timer::TotalTime() const
	{
		if (m_stoped)
		{
			return ((m_stopTime - m_baseTime - m_pausedTime) * m_secondsPerCount);
		}
		else
		{
			return ((m_currTime - m_baseTime - m_pausedTime) * m_secondsPerCount);
		}
	}
	double Timer::GetTickSeconds() const
	{
		return m_secondsPerCount;
	}
	void Timer::Reset()
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
		m_baseTime = currTime;
		m_stopTime = 0;
		m_prevTime = currTime;
		m_stoped = false;
	}
	void Timer::Start()
	{
		__int64 startTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
		std::cout << "started\n";
		if (m_stoped)
		{
			m_pausedTime += (startTime - m_stopTime);
			m_prevTime = startTime;
			m_stopTime = 0;
			m_stoped = false;
		}
	}
	void Timer::Stop()
	{
		if (!m_stoped)
		{
			__int64 currTime;
			QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
			m_stopTime = currTime;
			m_stoped = true;
			std::cout << "stoped\n";
		}
	}
	void Timer::Tick()
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
		m_prevTime = m_currTime;
		m_currTime = currTime;
		mDeltaTime = (m_currTime - m_prevTime) * m_secondsPerCount;
	}
}