//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Util/Time.h>
/******* C++ headers *******/
#include <chrono>
#include <Window/myWindows.h>
/******* extra headers *******/
/******* end headers *******/
	
namespace Core
{

#define QPCC 0
#define CHRC 1

#define TIME_SOURCE CHRC

	/********************************************************
	*		TIME SOURCES
	********************************************************/

#if (TIME_SOURCE == QPCC)
#define CLOCK_TYPE HighResClock

	const long long g_Frequency = []() -> long long
	{
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		return frequency.QuadPart;
	}();

	struct HighResClock
	{
		typedef long long                               rep;
		typedef std::nano                               period;
		typedef std::chrono::duration<rep, period>      duration;
		typedef std::chrono::time_point<HighResClock>   time_point;
		static const bool is_steady = true;

		static time_point now()
		{
			LARGE_INTEGER count;
			QueryPerformanceCounter(&count);
			return time_point(duration(count.QuadPart * static_cast<rep>(period::den) / g_Frequency));
		}
	};
#elif (TIME_SOURCE == CHRC)
#define CLOCK_TYPE std::chrono::high_resolution_clock
#endif


	
	class TimeSource
	{
	public:
		uint64_t getSystemMicroseconds()
		{
			auto now = CLOCK_TYPE::now();
			return std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
		}
	};



	/********************************************************
	*		TIME CLASS
	********************************************************/

	const double Time::STOP_TIME = 0;
	const double Time::NORMAL_TIME = 1;
	const float Time::m_microToSec = static_cast<float>(1.0 / 1000000.0);
	
	uint64_t Time::secondsToMicros(float sec)
	{
		return static_cast<uint64_t>(sec * 1000.0f * 1000.0f);
	}

	uint64_t Time::milisToMicros(uint32_t mili)
	{
		return static_cast<uint64_t>(mili) * 1000ULL;
	}

	float Time::milisToSeconds(uint32_t mili)
	{
		return static_cast<float>(mili)*0.001f;
	}

	float Time::microsToSeconds(uint64_t micros)
	{
		return static_cast<float>(micros)*0.001f*0.001f;
	}
	
	uint32_t Time::secondsToMilis(float sec)
	{
		return static_cast<uint32_t>(sec * 1000ULL);
	}
	
	uint32_t Time::microsToMilis(uint64_t micros)
	{
		return static_cast<uint32_t>(micros / 1000ULL);
	}

	uint32_t Time::microsDelta(uint64_t start, uint64_t end)
	{
		return static_cast<uint32_t>(end - start);
	}

	uint32_t Time::countMilisInMicros(uint64_t micros)
	{
		return static_cast<uint32_t>(micros / 1000ULL);
	}

	uint32_t Time::countSecondsInMicros(uint64_t micros)
	{
		return static_cast<uint32_t>(micros / 1000000ULL);
	}

	uint32_t Time::countMinutesInMicros(uint64_t micros)
	{
		return static_cast<uint32_t>(micros / (60ULL * 1000000ULL));
	}

	uint32_t Time::countHoursInMicros(uint64_t micros)
	{
		return static_cast<uint32_t>(micros / (60ULL * 60ULL * 1000000ULL));
	}

	Time::Time()
		: m_oldRealTime(getRealTimeMicros()),
		m_lastMicros(m_oldRealTime), m_curMicros(m_oldRealTime), m_deltaMicros(0),
		m_deltaTime(0), m_timeScale(NORMAL_TIME)
	{
	}

	uint64_t Time::getRealTimeMicros() const
	{
		TimeSource source;
		return source.getSystemMicroseconds();
	}

	uint64_t Time::getLastRealTimeMicros() const
	{
		return m_oldRealTime;
	}

	void Time::update()
	{
		uint64_t now = getRealTimeMicros();
		uint64_t delta = now - m_oldRealTime;
		m_oldRealTime = now;

		if(delta)
			updateBy(delta);
	}

	void Time::updateBy(uint64_t deltaMicros)
	{
		m_lastMicros = m_curMicros;
		m_curMicros += deltaMicros;
		m_deltaMicros = m_curMicros - m_lastMicros;
		m_deltaTime = static_cast<float>(m_deltaMicros) * m_microToSec;
	}

	uint32_t Time::getFixedStepUpdateCount(uint64_t frameTime, float& ratio, uint64_t& remainderTime)
	{
		uint64_t now = getRealTimeMicros();
		uint64_t delta = now - m_oldRealTime;
      
		uint32_t count = static_cast<uint32_t>(delta / frameTime);
		uint64_t addMe = count * frameTime;
		m_oldRealTime += addMe;
		remainderTime = delta - addMe;
		
		ratio = static_cast<float>(static_cast<double>(addMe) / static_cast<double>(frameTime));
		return count;
	}

	uint64_t Time::getCurMicros() const
	{
		return m_curMicros;
	}

	uint32_t Time::getDeltaMicros() const
	{
		return static_cast<uint32_t>(m_deltaMicros);
	}

	float Time::getDeltaTime() const
	{
		return m_deltaTime;
	}

	void Time::setTimeScale(double timeScale)
	{
		m_timeScale = timeScale;
	}

	double Time::getTimeScale() const
	{
		return m_timeScale;
	}

	void Time::reset()
	{
		m_curMicros = 0;
		m_deltaMicros = 0;
		m_deltaTime = 0;
		m_lastMicros = 0;
	}
}