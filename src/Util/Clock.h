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
	class Clock
	{
	public:
		static const double STOP_TIME;
		static const double NORMAL_TIME;

		static uint64_t secondsToMicros(float sec);
		static uint64_t milisToMicros(uint32_t mili);
		static float milisToSeconds(uint32_t mili);
		static float microsToSeconds(uint64_t micros);
		static uint32_t secondsToMilis(float sec);
		static uint32_t microsToMilis(uint64_t micros);
		
		static uint32_t countMilisInMicros(uint64_t micros);
		static uint32_t countSecondsInMicros(uint64_t micros);
		static uint32_t countMinutesInMicros(uint64_t micros);
		static uint32_t countHoursInMicros(uint64_t micros);

		static uint32_t microsDelta(uint64_t start, uint64_t end);

		static uint64_t getRealTimeMicros();
		
		Clock();

		void update();
		void updateBy(uint64_t delta);
		uint32_t getFixedStepUpdateCount(uint64_t frameTime, float& ratio, uint64_t& remainderTime);

		uint64_t getLastRealTimeMicros() const;
		
		uint64_t getCurMicros() const;
		uint32_t getDeltaMicros() const;
		float getDeltaTime() const;

	protected:
		uint64_t m_oldRealTime;
		
		uint64_t m_lastMicros;
		uint64_t m_curMicros;
		uint64_t m_deltaMicros;
		
		float m_deltaTime;
	};
}
