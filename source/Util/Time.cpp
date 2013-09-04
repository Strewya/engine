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
	uint64_t HighPrecisionTimeSource::getMiliseconds()
	{
		auto now = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
	}




	uint32_t GameClock::m_Framerate = 60;
	uint32_t GameClock::m_TimePerFrame = 1000/60;

	void GameClock::setFramerate(uint32_t framerate)
	{
		m_Framerate = framerate;
		m_TimePerFrame = 1000/framerate;
	}

	uint32_t GameClock::getFramerate()
	{
		return m_Framerate;
	}

	uint32_t GameClock::getTimePerFrame()
	{
		return m_TimePerFrame;
	}

	GameClock::GameClock(IExternalTimeSource& timeSource)
		: m_timeSource(timeSource), m_created(timeSource.getMiliseconds()), m_currentTime(0)
	{}

	uint32_t GameClock::createTimingData()
	{
		for(auto it = m_timers.begin(); it != m_timers.end(); ++it)
		{
			if(*it == nullptr)
			{
				it->reset(new TimingData());
				return it - m_timers.begin();
			}
		}
		m_timers.push_back(std::unique_ptr<TimingData>(new TimingData()));
		return m_timers.size() - 1;
	}

	uint32_t GameClock::createCountdownTimer(uint32_t initialTime, TimingData::Callback onOccurence, bool repeats)
	{
		uint32_t timerId = createTimingData();
		TimingData& timer = *m_timers[timerId];
		timer.m_type = TimerType::Countdown;
		timer.m_initialTime = m_currentFrame = initialTime;
		timer.m_onOccurence = onOccurence;
		timer.m_repeating = repeats;
		timer.m_done = false;
		return timerId;
	}

	void GameClock::stepGameTime()
	{
		m_lastUpdate = m_currentTime;
		m_currentTime = m_timeSource.getMiliseconds();

		uint32_t delta = uint32_t(m_currentTime - m_lastUpdate);
		clamp(delta, 0U, m_TimePerFrame*8);

		m_frameAccumulator += delta;
		uint32_t framesElapsed = (m_frameAccumulator / m_TimePerFrame);
		m_currentFrame += framesElapsed;
		m_frameAccumulator -= framesElapsed*m_TimePerFrame;

		removeFinishedTimers();
		updateTimers(delta);
		
	}

	void GameClock::updateTimers(uint32_t delta)
	{
		for(auto& unique : m_timers)
		{
			TimingData& timer = *unique;
			switch(timer.m_type)
			{
			case TimerType::Countdown:
				timer.m_currentTime -= delta;
				if(timer.m_currentTime <= 0)
				{
					timer.m_done = true;
				}
				break;
			}

			if(timer.m_done)
			{
				timer.m_onOccurence();
				if(timer.m_repeating)
				{
					timer.m_currentTime = timer.m_initialTime;
					timer.m_done = false;
				}
			}
		}
	}

	void GameClock::removeFinishedTimers()
	{
		for(auto& timer : m_timers)
		{
			if(timer == nullptr)
			{
				timer.reset(nullptr);
			}
		}
	}

	uint32_t GameClock::getCurrentDelta() const
	{
		return m_frameAccumulator;
	}

	uint32_t GameClock::getCurrentFrame() const
	{
		return m_currentFrame;
	}

	uint64_t GameClock::getCurrentGameTime() const
	{
		return m_currentTime - m_created;
	}
}