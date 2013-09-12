#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include <stdint.h>
#include <chrono>
#include <vector>
#include <memory>
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Util
{
	/********************************************************
	*		INTERFACES
	********************************************************/

		//	TIME SOURCE
	class IExternalTimeSource
	{
	public:
		virtual uint64_t getMiliseconds() = 0;
		virtual uint64_t getMicroseconds() = 0;
	};




	/********************************************************
	*		TIME SOURCES
	********************************************************/
	
		//	HIGH PRECISION SOURCE
	class HighPrecisionTimeSource : public IExternalTimeSource
	{
	public:
		uint64_t getMiliseconds();
		uint64_t getMicroseconds();
	};

	


	/********************************************************
	*		TIMING DATA
	********************************************************/

	enum class TimerType
	{
		Countdown = 0,
		Stopwatch,
		Accumulator,
		TIMER_TYPE_COUNT
	};

	

	struct TimerData
	{
		typedef std::function<void(void)> Callback;

		TimerType m_type;
		const uint32_t m_id;
		int32_t m_parent;
		uint32_t m_targetTime;
		uint32_t m_currentTime;
		bool m_repeating;
		bool m_done;
		bool m_paused;
		Callback m_onOccurrence;
		Callback m_onReset;

		//helper functions
		TimerData(uint32_t id);
		void reset();
		void pause();
		void resume();
	};



	/********************************************************
	*		CLOCK
	********************************************************/

	class GameClock
	{
	public:
		GameClock(IExternalTimeSource& timeSource);
		void stepGameTime();
		uint64_t getCurrentGameTime() const;
		void setFramerate(uint32_t framerate);
		uint32_t getFramerate() const;
		uint32_t getTimePerFrame() const;
		

		uint32_t createCountdownTimer(uint32_t startingTime, TimerData::Callback onOccurrence);
		uint32_t createStopwatch();
		uint32_t createAccumulator(uint32_t timeToFire, TimerData::Callback onOccurrence);

		void slaveTimers(uint32_t master, uint32_t slave);
		void unslaveTimer(uint32_t slave);

		bool isTimerAlive(uint32_t timerId) const;
		void deleteTimer(uint32_t timerId);
		TimerData& getTimer(uint32_t timerId);

	private:
		IExternalTimeSource& m_timeSource;
		const uint64_t m_createdRealTime;
		uint32_t m_framerate;
		uint32_t m_timePerFrame;
		uint64_t m_currentGameTime;
		uint64_t m_lastUpdateRealTime;
		std::vector<std::unique_ptr<TimerData>> m_timers;

		TimerData& createTimingData();
		void updateTimers(uint32_t delta);
		void removeFinishedTimers();
		bool isActive(uint32_t timerId) const;
	};
}
