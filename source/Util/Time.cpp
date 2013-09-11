//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Util/Time.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Engine/Defines.h"
	/*** end headers ***/
	
namespace Util
{
#ifdef _DEBUG
	static const uint32_t CLAMP_FRAME_COUNT = 1;
#else
	static const uint32_t CLAMP_FRAME_COUNT = 8;
#endif

	uint64_t HighPrecisionTimeSource::getMiliseconds()
	{
		auto now = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
	}

	uint64_t HighPrecisionTimeSource::getMicroseconds()
	{
		auto now = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
	}


	TimerData::TimerData()
		: m_type(TimerType::Stopwatch), m_targetTime(0), m_currentTime(0),
		m_repeating(false), m_done(false), m_paused(false),
		m_onOccurrence([](){}), m_onReset([&](){m_currentTime = m_targetTime;})
	{}

	void TimerData::reset()
	{
		m_done = false;
		m_onReset();
	}

	void TimerData::pause()
	{
		m_paused = true;
	}

	void TimerData::resume()
	{
		m_paused = false;
	}




	GameClock::GameClock(IExternalTimeSource& timeSource)
		: m_timeSource(timeSource), m_createdRealTime(timeSource.getMiliseconds()),
		m_framerate(60), m_timePerFrame(1000/m_framerate), m_currentGameTime(0), m_lastUpdateRealTime(0)
	{}

	void GameClock::setFramerate(uint32_t framerate)
	{
		m_framerate = framerate;
		m_timePerFrame = 1000/framerate;
	}

	uint32_t GameClock::getFramerate() const
	{
		return m_framerate;
	}

	uint32_t GameClock::getTimePerFrame() const
	{
		return m_timePerFrame;
	}

	uint32_t GameClock::createTimingData()
	{
		for(auto it = m_timers.begin(); it != m_timers.end(); ++it)
		{
			if(*it == nullptr)
			{
				it->reset(new TimerData());
				return it - m_timers.begin();
			}
		}
		m_timers.push_back(std::unique_ptr<TimerData>(new TimerData()));
		return m_timers.size() - 1;
	}

	uint32_t GameClock::createCountdownTimer(uint32_t initialTime, TimerData::Callback onOccurence)
	{
		uint32_t timerId = createTimingData();
		TimerData& timer = *m_timers[timerId];
		timer.m_type = TimerType::Countdown;
		timer.m_targetTime = initialTime;
		timer.m_onOccurrence = onOccurence;
		return timerId;
	}

	uint32_t GameClock::createStopwatch()
	{
		uint32_t timerId = createTimingData();
		TimerData& timer = *m_timers[timerId];
		timer.m_type = TimerType::Stopwatch;
		return timerId;
	}

	uint32_t GameClock::createAccumulator(uint32_t timeToFire, TimerData::Callback onOccurrence)
	{
		uint32_t timerId = createTimingData();
		TimerData& timer = *m_timers[timerId];
		timer.m_type = TimerType::Accumulator;
		timer.m_targetTime = timeToFire;
		timer.m_onOccurrence = onOccurrence;
		timer.m_onReset = [](){};
		timer.m_repeating = true;
		return timerId;
	}

	bool GameClock::isTimerAlive(uint32_t timerId) const
	{
		return timerId < m_timers.size() && m_timers[timerId] != nullptr;
	}

	void GameClock::deleteTimer(uint32_t timerId)
	{
		if(isTimerAlive(timerId))
		{
			m_timers[timerId].reset(nullptr);
		}
	}

	TimerData& GameClock::getTimer(uint32_t timerId)
	{
		return *m_timers[timerId];
	}

	void GameClock::stepGameTime()
	{
		//get the elapsed real time
		uint64_t currentRealTime = m_timeSource.getMiliseconds();
		uint32_t elapsedRealTime = uint32_t(currentRealTime - m_lastUpdateRealTime);
		m_lastUpdateRealTime = currentRealTime;

		//calculate the game time delta by clamping the real elapsed time to a couple of frames worth of time
		uint32_t gameDelta = clamp(elapsedRealTime, 0U, m_timePerFrame*CLAMP_FRAME_COUNT);

		m_currentGameTime += gameDelta;

		//first we remove the timers that were finished last frame and then update the remining timers
		// this is to allow timers that finish this frame to be reset by their owner if they wish
		removeFinishedTimers();
		updateTimers(gameDelta);
	}

	void GameClock::updateTimers(uint32_t delta)
	{
		static std::function<void(TimerData&, uint32_t)> timingUpdaters[(uint32_t)TimerType::TIMER_TYPE_COUNT] =
		{
			[](TimerData& t, uint32_t dt){ t.m_currentTime -= dt; if(t.m_currentTime <= 0) { t.m_done = true; } },
			[](TimerData& t, uint32_t dt){ t.m_currentTime += dt; },
			[](TimerData& t, uint32_t dt){ t.m_currentTime += dt; if(t.m_currentTime >= t.m_targetTime) { t.m_done = true; } },
		};

		for(auto& uptr : m_timers)
		{
			if(uptr != nullptr && !uptr->m_paused)
			{
				TimerData& timer = *uptr;
				
				timingUpdaters[(uint32_t)timer.m_type](timer, delta);
				
				if(timer.m_done)
				{
					timer.m_onOccurrence();
					if(timer.m_repeating)
					{
						timer.reset();
					}
				}
			}
		}
	}

	void GameClock::removeFinishedTimers()
	{
		for(auto& timer : m_timers)
		{
			if(timer != nullptr && timer->m_done)
			{
				timer.reset(nullptr);
			}
		}
	}

	uint64_t GameClock::getCurrentGameTime() const
	{
		return m_currentGameTime;
	}
}