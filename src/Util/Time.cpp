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
   *		TIME CLASSES
   ********************************************************/

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


   Timer::Timer() :
      m_currentMicros(0),
      m_deltaMicros(0),
      m_deltaSeconds(0),
      m_timeScale(1)
   {}

   void Timer::updateBy(uint32_t deltaMicros)
   {
      m_deltaMicros = static_cast<uint32_t>(static_cast<float>(deltaMicros)*m_timeScale);
      m_currentMicros += m_deltaMicros;
      m_deltaSeconds = microsToSeconds(m_deltaMicros);
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
         m_currentMicros = 0;
      }
      else
      {
         m_currentMicros -= micros;
      }
   }


   CooldownTimer::CooldownTimer()
      : CooldownTimer(0)
   {}

   CooldownTimer::CooldownTimer(uint32_t durationMicros)
      : m_durationMicros(durationMicros)
   {}

   void CooldownTimer::updateBy(uint32_t deltaMicros)
   {
      m_timer.updateBy(deltaMicros);
   }
   bool CooldownTimer::hasElapsed() const
   {
      return m_timer.getCurrentMicros() >= m_durationMicros;
   }
   uint32_t CooldownTimer::getRemainingMicros() const
   {
      return m_durationMicros - m_timer.getCurrentMicros();
   }
   float CooldownTimer::getPercentDone() const
   {
      return static_cast<float>(m_timer.getCurrentMicros()) / static_cast<float>(m_durationMicros);
   }
   void CooldownTimer::reset()
   {
      m_timer.reset();
   }

   uint32_t CooldownTimer::getDurationMicros() const
   {
      return m_durationMicros;
   }
   void CooldownTimer::setDurationMicros(uint32_t durationMicros)
   {
      m_durationMicros = durationMicros;
   }
   const Timer& CooldownTimer::getTimer() const
   {
      return m_timer;
   }


   PeriodicTimer::PeriodicTimer()
      : PeriodicTimer(0)
   {}

   PeriodicTimer::PeriodicTimer(uint32_t periodMicros)
      : m_periodMicros(periodMicros)
   {}

   void PeriodicTimer::updateBy(uint32_t deltaMicros)
   {
      m_timer.updateBy(deltaMicros);
   }
   bool PeriodicTimer::hasElapsed() const
   {
      return m_timer.getCurrentMicros() >= m_periodMicros;
   }
   uint32_t PeriodicTimer::getRemainingMicros() const
   {
      return m_periodMicros - m_timer.getCurrentMicros();
   }
   float PeriodicTimer::getPercentDone() const
   {
      return static_cast<float>(m_timer.getCurrentMicros()) / static_cast<float>(m_periodMicros);
   }
   void PeriodicTimer::period()
   {
      m_timer.reset(m_periodMicros);
   }
   void PeriodicTimer::hardReset()
   {
      m_timer.reset();
   }

   uint32_t PeriodicTimer::getPeriodMicros() const
   {
      return m_periodMicros;
   }
   void PeriodicTimer::setPeriodMicros(uint32_t periodMicros)
   {
      m_periodMicros = periodMicros;
   }
   const Timer& PeriodicTimer::getTimer() const
   {
      return m_timer;
   }



   uint32_t secondsToMicros(uint32_t s)
   {
      return s * 1000U * 1000U;
   }
   uint32_t secondsToMicros(float s)
   {
      return static_cast<uint32_t>(s*1000.0f*1000.0f);
   }
   uint32_t secondsToMilis(uint32_t s)
   {
      return s * 1000U;
   }
   uint32_t secondsToMilis(float s)
   {
      return static_cast<uint32_t>(s*1000.0f);
   }
   uint32_t milisToMicros(uint32_t ms)
   {
      return ms * 1000U;
   }
   float milisToSeconds(uint32_t ms)
   {
      return static_cast<float>(ms)*0.001f;
   }
   float microsToSeconds(uint32_t us)
   {
      return static_cast<float>(us)*0.001f*0.001f;
   }
}