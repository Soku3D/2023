#include <Windows.h>

namespace soku {
	class Timer {
	public:
		Timer();

		float GetTime()const;
		float DeltaTime()const;
		float TotalTime()const;
		double GetTickSeconds()const;
		void Reset();
		void Start();
		void Stop();
		void Tick();

	private:
		double m_secondsPerCount;
		double mDeltaTime;

		__int64 m_baseTime;
		__int64 m_pausedTime;
		__int64 m_stopTime;
		__int64 m_prevTime;
		__int64 m_currTime;

		bool m_stoped;

	};
}