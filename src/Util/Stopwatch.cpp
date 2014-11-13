//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Util/Stopwatch.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Util/Clock.h>
/******* end headers *******/

namespace Core
{
	Stopwatch::Stopwatch()
		: m_currentMicros(0),
		m_oldMicros(0),
		m_deltaMicros(0),
		m_timeScale(Clock::NORMAL_TIME),
		m_deltaTime(0)
	{}

	void Stopwatch::updateBy(int64_t deltaMicros)
	{
		m_oldMicros = m_currentMicros;
		m_currentMicros += static_cast<int64_t>(static_cast<double>(deltaMicros)*m_timeScale);
		m_deltaMicros = m_currentMicros - m_oldMicros;
		m_deltaTime = Clock::microsToSeconds(m_deltaMicros);
	}

	void Stopwatch::reset()
	{
		m_oldMicros = m_currentMicros = m_deltaMicros = 0;
		m_timeScale = 0;
		m_deltaTime = 0;
	}

	void Stopwatch::setTimeScale(double scale)
	{
		m_timeScale = scale;
	}

	double Stopwatch::getTimeScale() const
	{
		return m_timeScale;
	}
		
	int64_t Stopwatch::getCurrentMicros() const
	{
		return m_currentMicros;
	}
	
	int32_t Stopwatch::getDeltaMicros() const
	{
		return static_cast<int32_t>(m_deltaMicros);
	}
	
	float Stopwatch::getDeltaTime() const
	{
		return m_deltaTime;
	}
}
