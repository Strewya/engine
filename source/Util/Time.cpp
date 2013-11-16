//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Util/Time.h"
	/*** C++ headers ***/
#include <chrono>
	/*** extra headers ***/
#include "Engine/Defines.h"
	/*** end headers ***/
	
namespace Util
{
	/********************************************************
	*		TIME SOURCES
	********************************************************/
	
	class HighPrecisionTimeSource
	{
	public:
		uint64_t getSystemMiliseconds()
		{
			auto now = std::chrono::high_resolution_clock::now();
			return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
		}
		uint64_t getSystemMicroseconds()
		{
			auto now = std::chrono::high_resolution_clock::now();
			return std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
		}
	};

	class QPCTimeSource
	{
	public:
		QPCTimeSource();
		uint64_t getSystemMiliseconds()
		{
			return 0;
		}
		uint64_t getSystemMicroseconds()
		{
			return 0;
		}
	private:

	};

#define QPCTS QPCTimeSource
#define HPTS HighPrecisionTimeSource
#define TIME_SOURCE HPTS




	/********************************************************
	*		TIME CLASS
	********************************************************/

	const double Time::STOP_TIME = 0;
	const double Time::NORMAL_TIME = 1;

	Time::Time()
		: m_oldRealTime(getRealTimeMicros()),
		m_lastMicros(0), m_curMicros(0), m_deltaMicros(0), m_deltaTime(0),
		m_virtLastMicros(0), m_virtCurMicros(0), m_virtDeltaMicros(0), m_virtDeltaTime(0),
		m_microToSec(static_cast<float>(1.0/1000000.0))
	{
	}

	uint64_t Time::getRealTimeMicros() const
	{
		TIME_SOURCE source;
		return source.getSystemMicroseconds();
	}

	uint64_t Time::getLastRealTimeMicros() const
	{
		return m_oldRealTime;
	}

	void Time::update(double virtualTimeScale)
	{
		uint64_t now = getRealTimeMicros();
		uint64_t delta = now - m_oldRealTime;
		m_oldRealTime = now;

		updateBy(delta, virtualTimeScale);
	}

	void Time::updateBy(uint64_t deltaMicros, double virtualTimeScale)
	{
		m_lastMicros = m_curMicros;
		m_curMicros += deltaMicros;
		m_deltaMicros = m_curMicros - m_lastMicros;
		m_deltaTime = static_cast<float>(m_deltaMicros) * m_microToSec;

		if(virtualTimeScale > 0) //zero is full stop, less would mean rewind, which is impossible?
		{
			m_virtLastMicros = m_virtCurMicros;
			m_virtCurMicros += static_cast<uint64_t>(static_cast<double>(deltaMicros) * virtualTimeScale);
			m_virtDeltaMicros = m_virtCurMicros - m_virtLastMicros;
			m_virtDeltaTime = static_cast<float>(m_virtDeltaMicros) * m_microToSec;
		}
	}

	uint32_t Time::getFixedStepUpdateCount(uint64_t time, float& ratio, uint64_t& remainderTime)
	{
		uint64_t now = getRealTimeMicros();
		uint64_t delta = now - m_oldRealTime;
      
		uint32_t count = static_cast<uint32_t>(delta / time);
		uint64_t addMe = count * time;
		m_oldRealTime += addMe;
		remainderTime = delta - addMe;
       
		ratio = static_cast<float>(static_cast<double>(addMe) / static_cast<double>(time));
		return count;
	}

	uint64_t Time::getCurMicros() const
	{
		return m_curMicros;
	}

	uint64_t Time::getDeltaMicros() const
	{
		return m_deltaMicros;
	}

	float Time::getDeltaTime() const
	{
		return m_deltaTime;
	}

	uint64_t Time::getVirtCurMicros() const
	{
		return m_virtCurMicros;
	}

	uint64_t Time::getVirtDeltaMicros() const
	{
		return m_virtDeltaMicros;
	}

	float Time::getVirtDeltaTime() const
	{
		return m_virtDeltaTime;
	}
}