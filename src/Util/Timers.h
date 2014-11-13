#pragma once
/********************************************
*	class:	Timer
*	usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
	class DecrementingTimer
	{
	public:
		DecrementingTimer();

		void startMicros(uint64_t duration);
		void startMilis(uint32_t duration);
		void startSeconds(float duration);

		void updateBy(int64_t deltaMicros);

		void setTimeScale(double scale);
		double getTimeScale() const;
		uint64_t getStartingValue();
		int64_t getRemainingMicros() const;
		bool isDone() const;

		void resetToStartingValue();
		void addExtraMicros(uint64_t extraMicros);

	private:
		int64_t m_remaining;
		uint64_t m_startingValue;
		double m_timeScale;
	};

	class IncrementingTimer
	{
	public:
		IncrementingTimer();

		void updateBy(int64_t deltaMicros);

		void setTimeScale(double scale);
		double getTimeScale() const;

		int32_t getCurrentMicros() const;
		int32_t getDeltaMicros() const;
		float getDeltaTime() const;

		void reset();

	private:
		int64_t m_currentMicros;
		int64_t m_oldMicros;
		int64_t m_deltaMicros;
		double m_timeScale;
		float m_deltaTime;
	};

	class IntervalTimer
	{
	public:
		IntervalTimer();

		void setIntervalMicros(uint64_t interval);
		void setIntervalMilis(uint32_t interval);
		void setIntervalSeconds(float interval);

		void updateBy(int64_t deltaMicros);

		void setTimeScale(double scale);
		double getTimeScale() const;

		bool isIntervalPassed();

	private:
		int64_t m_currentMicros;
		int64_t m_intervalMicros;
		double m_timeScale;
		bool m_intervalPassed;
	};
}
