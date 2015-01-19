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

   int64_t Time::secondsToMicros(float sec)
   {
      return static_cast<int64_t>(sec * 1000.0f * 1000.0f);
   }

   int64_t Time::milisToMicros(int32_t mili)
   {
      return static_cast<int64_t>(mili)* 1000ULL;
   }

   float Time::milisToSeconds(int32_t mili)
   {
      return static_cast<float>(mili)*0.001f;
   }

   float Time::microsToSeconds(int64_t micros)
   {
      return static_cast<float>(micros)*0.001f*0.001f;
   }

   int32_t Time::secondsToMilis(float sec)
   {
      return static_cast<int32_t>(sec * 1000ULL);
   }

   int32_t Time::microsToMilis(int64_t micros)
   {
      return static_cast<int32_t>(micros / 1000ULL);
   }

   int32_t Time::microsDelta(int64_t start, int64_t end)
   {
      return static_cast<int32_t>(end - start);
   }

   int32_t Time::countMilisInMicros(int64_t micros)
   {
      return static_cast<int32_t>(micros / 1000ULL);
   }

   int32_t Time::countSecondsInMicros(int64_t micros)
   {
      return static_cast<int32_t>(micros / 1000000ULL);
   }

   int32_t Time::countMinutesInMicros(int64_t micros)
   {
      return static_cast<int32_t>(micros / (60ULL * 1000000ULL));
   }

   int32_t Time::countHoursInMicros(int64_t micros)
   {
      return static_cast<int32_t>(micros / (60ULL * 60ULL * 1000000ULL));
   }

   Time::Time()
      : m_oldRealTime(getRealTimeMicros()),
      m_lastMicros(m_oldRealTime), m_curMicros(m_oldRealTime), m_deltaMicros(0),
      m_deltaTime(0), m_timeScale(NORMAL_TIME)
   {}

   uint64_t Time::getRealTimeMicros()
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

      if( delta )
         updateBy(delta);
   }

   void Time::updateBy(int64_t deltaMicros)
   {
      m_lastMicros = m_curMicros;
      m_curMicros += static_cast<int64_t>(static_cast<double>(deltaMicros)*m_timeScale);
      m_deltaMicros = m_curMicros - m_lastMicros;
      m_deltaTime = microsToSeconds(m_deltaMicros);
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

   uint64_t Time::getCurrentMicros() const
   {
      return m_curMicros;
   }

   int32_t Time::getDeltaMicros() const
   {
      return static_cast<int32_t>(m_deltaMicros);
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







   Clock::Clock()
      : m_oldRealTime(getRealTimeMicros()),
      m_timeScale(1),
      m_lastMicros(m_oldRealTime),
      m_currentMicros(m_lastMicros),
      m_deltaMicros(0),
      m_deltaSeconds(0)
   {}

   void Clock::update()
   {
      uint64_t now = getRealTimeMicros();
      uint64_t delta = now - m_oldRealTime;
      m_oldRealTime = now;

      if( delta )
      {
         updateBy(delta);
      }
   }

   void Clock::updateBy(uint64_t deltaMicros)
   {
      m_lastMicros = m_currentMicros;
      m_currentMicros += static_cast<uint64_t>(static_cast<double>(deltaMicros)*m_timeScale);
      m_deltaMicros = m_currentMicros - m_lastMicros;
      m_deltaSeconds = static_cast<float>(m_deltaMicros)*0.001f*0.001f;
   }

   uint32_t Clock::getFixedStepUpdateCount(uint64_t frameTime, float& ratio, uint64_t& remainderTime)
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

   uint64_t Clock::getRealTimeMicros()
   {
      TimeSource source;
      return source.getSystemMicroseconds();
   }

   uint64_t Clock::getLastRealTimeMicros() const
   {
      return m_oldRealTime;
   }

   uint64_t Clock::getCurrentMicros() const
   {
      return m_currentMicros;
   }

   uint32_t Clock::getDeltaMicros() const
   {
      return static_cast<uint32_t>(m_deltaMicros);
   }

   float Clock::getDeltaSeconds() const
   {
      return m_deltaSeconds;
   }

   void Clock::setTimeScale(double timeScale)
   {
      m_timeScale = timeScale;
   }

   double Clock::getTimeScale() const
   {
      return m_timeScale;
   }


   Timer::Timer()
      : m_lastMicros(0),
      m_currentMicros(0),
      m_deltaMicros(0),
      m_deltaSeconds(0),
      m_timeScale(1)
   {}

   void Timer::updateBy(uint32_t deltaMicros)
   {
      m_lastMicros = m_currentMicros;
      m_currentMicros += static_cast<uint32_t>(static_cast<float>(deltaMicros)*m_timeScale);
      m_deltaMicros = m_currentMicros - m_lastMicros;
      m_deltaSeconds = static_cast<float>(m_deltaMicros)*0.001f*0.001f;
   }

   void Timer::setTimeScale(float timeScale)
   {
      m_timeScale = timeScale;
   }

   float Timer::getTimeScale() const
   {
      return m_timeScale;
   }

   uint32_t Timer::getCurrentMicros() const
   {
      return m_currentMicros;
   }

   uint32_t Timer::getDeltaMicros() const
   {
      return m_deltaMicros;
   }

   float Timer::getDeltaSeconds() const
   {
      return m_deltaSeconds;
   }

   void Timer::reset(uint32_t micros)
   {
      if( micros == 0 )
      {
         m_lastMicros = 0;
         m_currentMicros = 0;
      }
      else
      {
         m_lastMicros -= micros;
         m_currentMicros -= micros;
      }
   }
}