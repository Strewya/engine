//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "util/time/clock.h"
/******* c++ headers *******/
#include <chrono>
/******* extra headers *******/
#include "util/utility.h"
/******* end headers *******/

namespace core
{

#define QPCC 0
#define CHRC 1

#define TIME_SOURCE QPCC

   /********************************************************
   *     TIME SOURCES
   ********************************************************/

#if (TIME_SOURCE == QPCC)
#define CLOCK_TYPE HighResClock

#include "Window/window_include.h"
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
      core_class_scope const bool is_steady = true;


      core_class_scope time_point now()
      {
         LARGE_INTEGER count;
         QueryPerformanceCounter(&count);
         return time_point(duration(count.QuadPart * static_cast<rep>(period::den) / g_Frequency));
      }
   };
#elif (TIME_SOURCE == CHRC)
#define CLOCK_TYPE std::chrono::high_resolution_clock
#endif

   /********************************************************
   *     CLOCK IMPLEMENTATION
   ********************************************************/

   class TimeSource
   {
   public:
      u64 getSystemMicroseconds()
      {
         auto now = CLOCK_TYPE::now();
         return std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
      }
   };



   Clock::Clock()
      : m_oldRealTime(getRealTimeMicros()),
      m_timeScale(1),
      m_lastMicros(m_oldRealTime),
      m_currentMicros(m_lastMicros),
      m_deltaMicros(0),
      m_deltaSeconds(0)
   {
   }

   void Clock::update()
   {
      u64 now = getRealTimeMicros();
      u64 delta = now - m_oldRealTime;
      m_oldRealTime = now;

      if( delta )
      {
         advanceTimeBy(delta);
      }
   }

   void Clock::advanceTimeBy(u64 deltaMicros)
   {
      m_lastMicros = m_currentMicros;
      m_currentMicros += static_cast<u64>(static_cast<f64>(deltaMicros)*m_timeScale);
      m_deltaMicros = m_currentMicros - m_lastMicros;
      m_deltaSeconds = static_cast<f32>(m_deltaMicros)*0.001f*0.001f;
   }

   u32 Clock::getFixedStepUpdateCount(u64 frameTime, f32& ratio, u64& remainderTime)
   {
      u64 now = getRealTimeMicros();
      u64 delta = now - m_oldRealTime;

      u32 count = static_cast<u32>(delta / frameTime);
      u64 addMe = count * frameTime;
      m_oldRealTime += addMe;
      remainderTime = delta - addMe;

      ratio = static_cast<f32>(static_cast<f64>(addMe) / static_cast<f64>(frameTime));
      return count;
   }

   u64 Clock::getRealTimeMicros()
   {
      TimeSource source;
      return source.getSystemMicroseconds();
   }

   u64 Clock::getLastRealTimeMicros() const
   {
      return m_oldRealTime;
   }

   u64 Clock::getCurrentMicros() const
   {
      return m_currentMicros;
   }

   u32 Clock::getDeltaMicros() const
   {
      return static_cast<u32>(m_deltaMicros);
   }

   f32 Clock::getDeltaSeconds() const
   {
      return m_deltaSeconds;
   }

   void Clock::setTimeScale(f64 timeScale)
   {
      m_timeScale = timeScale;
   }

   f64 Clock::getTimeScale() const
   {
      return m_timeScale;
   }
}