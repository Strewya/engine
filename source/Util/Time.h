#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include <stdint.h>
#include <chrono>
#include <deque>
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
	};




	/********************************************************
	*		TIME SOURCES
	********************************************************/
	
		//	HIGH PRECISION SOURCE
	class HighPrecisionTimeSource : public IExternalTimeSource
	{
	public:
		uint64_t getMiliseconds();
	};

	


	/********************************************************
	*		TIMING DATA
	********************************************************/

	enum class TimerType
	{
		Countdown,
		Stopwatch,
	};

	

	struct TimingData
	{
		typedef std::function<void(void)> Callback;

		TimingData() : m_type(TimerType::Stopwatch), m_initialTime(0), m_currentTime(0) {}
		TimerType m_type;
		uint32_t m_initialTime;
		uint32_t m_currentTime;
		bool m_repeating;
		bool m_done;
		Callback m_onOccurence;
	};



	/********************************************************
	*		CLOCK
	********************************************************/

	class GameClock
	{
	public:
		static void setFramerate(uint32_t framerate);
		static uint32_t getFramerate();
		static uint32_t getTimePerFrame();
		


		GameClock(IExternalTimeSource& timeSource);
		void stepGameTime();
		uint32_t getCurrentDelta() const;
		uint32_t getCurrentFrame() const;
		uint64_t getCurrentGameTime() const;

		uint32_t createCountdownTimer(uint32_t startingTime, TimingData::Callback onOccurence, bool repeats = false);
		

	private:
		static uint32_t m_Framerate;
		static uint32_t m_TimePerFrame;

		IExternalTimeSource& m_timeSource;
		const uint64_t m_created;
		uint64_t m_currentTime;
		uint64_t m_lastUpdate;
		uint32_t m_frameAccumulator;
		uint32_t m_currentFrame;
		std::deque<std::unique_ptr<TimingData>> m_timers;

		uint32_t createTimingData();
		void updateTimers(uint32_t delta);
		void removeFinishedTimers();
	};
}
