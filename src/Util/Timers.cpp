//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Util/Timers.h>	
/******* C++ headers *******/
/******* extra headers *******/
#include <Util/Clock.h>
/******* end headers *******/

namespace Core
{
	DecrementingTimer::DecrementingTimer()
		: m_remaining(0),
		m_startingValue(0),
		m_timeScale(Clock::NORMAL_TIME)
	{}

	void DecrementingTimer::startMicros(uint64_t duration)
	{
		m_startingValue = m_remaining = duration;
	}

	void DecrementingTimer::startMilis(uint32_t duration)
	{
		m_startingValue = m_remaining = Clock::milisToMicros(duration);
	}

	void DecrementingTimer::startSeconds(float duration)
	{
		m_startingValue = m_remaining = Clock::secondsToMicros(duration);
	}

	void DecrementingTimer::updateBy(int64_t deltaMicros)
	{
		m_remaining -= static_cast<int64_t>(static_cast<double>(deltaMicros)*m_timeScale);
	}

	void DecrementingTimer::setTimeScale(double scale)
	{
		m_timeScale = scale;
	}

	double DecrementingTimer::getTimeScale() const
	{
		return m_timeScale;
	}

	uint64_t DecrementingTimer::getStartingValue()
	{
		return m_startingValue;
	}

	int64_t DecrementingTimer::getRemainingMicros() const
	{
		return m_remaining;
	}

	bool DecrementingTimer::isDone() const
	{
		return m_remaining <= 0;
	}

	void DecrementingTimer::resetToStartingValue()
	{
		m_remaining = m_startingValue;
	}

	void DecrementingTimer::addExtraMicros(uint64_t extraMicros)
	{
		m_remaining += extraMicros;
	}

	IncrementingTimer::IncrementingTimer()
		: m_currentMicros(0),
		m_oldMicros(0),
		m_deltaMicros(0),
		m_timeScale(Clock::NORMAL_TIME),
		m_deltaTime(0)
	{}

	void IncrementingTimer::updateBy(int64_t deltaMicros)
	{
		m_oldMicros = m_currentMicros;
		m_currentMicros += static_cast<int64_t>(static_cast<double>(deltaMicros)*m_timeScale);
		m_deltaMicros = m_currentMicros - m_oldMicros;
		m_deltaTime = Clock::microsToSeconds(m_deltaMicros);
	}

	void IncrementingTimer::setTimeScale(double scale)
	{
		m_timeScale = scale;
	}

	double IncrementingTimer::getTimeScale() const
	{
		return m_timeScale;
	}

	int32_t IncrementingTimer::getCurrentMicros() const
	{
		return static_cast<uint32_t>(m_currentMicros);
	}

	int32_t IncrementingTimer::getDeltaMicros() const
	{
		return static_cast<uint32_t>(m_deltaMicros);
	}

	float IncrementingTimer::getDeltaTime() const
	{
		return m_deltaTime;
	}

	void IncrementingTimer::reset()
	{
		m_currentMicros = m_oldMicros = m_deltaMicros = 0;
		m_deltaTime = 0;
	}

	IntervalTimer::IntervalTimer()
		: m_currentMicros(0),
		m_intervalMicros(0),
		m_timeScale(Clock::NORMAL_TIME),
		m_intervalPassed(false)
	{}

	void IntervalTimer::setIntervalMicros(uint64_t interval)
	{
		m_intervalMicros = interval;
	}

	void IntervalTimer::setIntervalMilis(uint32_t interval)
	{
		m_intervalMicros = Clock::milisToMicros(interval);
	}

	void IntervalTimer::setIntervalSeconds(float interval)
	{
		m_intervalMicros = Clock::secondsToMicros(interval);
	}

	void IntervalTimer::updateBy(int64_t deltaMicros)
	{
		m_intervalPassed = m_currentMicros >= m_intervalMicros;
		if(!m_intervalPassed)
		{
			m_currentMicros += static_cast<int64_t>(static_cast<double>(deltaMicros)*m_timeScale);
		}
	}

	void IntervalTimer::setTimeScale(double scale)
	{
		m_timeScale = scale;
	}

	double IntervalTimer::getTimeScale() const
	{
		return m_timeScale;
	}

	bool IntervalTimer::isIntervalPassed()
	{
		if(m_intervalPassed)
		{
			m_currentMicros -= m_intervalMicros;
			return true;
		}
		return false;
	}
}