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

	/*uint64_t Time::microsFromSeconds(uint32_t sec)
	{
		return static_cast<uint64_t>(sec) * 1000 * 1000;
	}*/

	uint64_t Time::microsFromSeconds(float sec)
	{
		return static_cast<uint64_t>(sec * 1000 * 1000);
	}

	uint64_t Time::microsFromMilis(uint32_t mili)
	{
		return static_cast<uint64_t>(mili) * 1000;
	}

	Time::Time()
		: m_oldRealTime(getRealTimeMicros()),
		m_lastMicros(0), m_curMicros(0), m_deltaMicros(0),
		m_virtLastMicros(0), m_virtCurMicros(0), m_virtDeltaMicros(0),
		m_deltaTime(0), m_virtDeltaTime(0),
		m_microToSec(static_cast<float>(1.0/1000000.0))
	{
	}

	uint64_t Time::getRealTimeMicros() const
	{
		static TimeSource source;
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

		if(delta)
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