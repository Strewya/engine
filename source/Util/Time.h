#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include <stdint.h>
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Util
{
	
	class Time
	{
	public:
		static const double STOP_TIME;
		static const double NORMAL_TIME;

		Time();

		void update(double virtualTimeScale);
		void updateBy(uint64_t delta, double virtualTimeScale);
		uint32_t getFixedStepUpdateCount(uint64_t time, float& ratio, uint64_t& remainderTime);

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
		float m_deltaTime;

		uint64_t m_virtLastMicros;
		uint64_t m_virtCurMicros;
		uint64_t m_virtDeltaMicros;
		float m_virtDeltaTime;

		float m_microToSec;
	};
}
