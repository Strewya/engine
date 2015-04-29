//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "util/clock.h"
/******* c++ headers *******/
#include <chrono>
/******* extra headers *******/
#include "util/utility.h"
/******* end headers *******/

namespace Core
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

   /********************************************************
   *     CLOCK IMPLEMENTATION
   ********************************************************/

   class TimeSource
   {
   public:
      uint64_t getSystemMicroseconds()
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
}