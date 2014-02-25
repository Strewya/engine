#pragma once
/********************************************
*	class:	Time
*	usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
	class Time
	{
	public:
		static const double STOP_TIME;
		static const double NORMAL_TIME;

		//static uint64_t microsFromSeconds(uint32_t sec);
		static uint64_t microsFromSeconds(float sec);
		static uint64_t microsFromMilis(uint32_t mili);

		Time();

		void update(double virtualTimeScale);
		void updateBy(uint64_t delta, double virtualTimeScale);
		uint32_t getFixedStepUpdateCount(uint64_t frameTime, float& ratio, uint64_t& remainderTime);

		uint64_t getLastRealTimeMicros() const;
		uint64_t getRealTimeMicros() const;
		
		uint64_t getCurMicros() const;
		uint64_t getDeltaMicros() const;
		float getDeltaTime() const;

		uint64_t getVirtCurMicros() const;
		uint64_t getVirtDeltaMicros() const;
		float getVirtDeltaTime() const;

	protected:
		uint64_t m_oldRealTime;
		
		uint64_t m_lastMicros;
		uint64_t m_curMicros;
		uint64_t m_deltaMicros;
		
		uint64_t m_virtLastMicros;
		uint64_t m_virtCurMicros;
		uint64_t m_virtDeltaMicros;
		
		float m_deltaTime;
		float m_virtDeltaTime;
		const float m_microToSec;
	};
}
