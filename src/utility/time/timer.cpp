//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "utility/time/timer.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "utility/time/time_unit_conversions.h"
/******* end headers *******/

namespace core
{
   Timer::Timer() :
      m_currentMicros(0),
      m_deltaMicros(0),
      m_deltaSeconds(0),
      m_timeScale(1)
   {
   }

   void Timer::advanceTimeBy(u32 deltaMicros)
   {
      m_deltaMicros = static_cast<u32>(static_cast<f32>(deltaMicros)*m_timeScale);
      m_currentMicros += m_deltaMicros;
      m_deltaSeconds = microsToSeconds(m_deltaMicros);
   }

   void Timer::setTimeScale(f32 timeScale)
   {
      m_timeScale = timeScale;
   }

   f32 Timer::getTimeScale() const
   {
      return m_timeScale;
   }

   u32 Timer::getCurrentMicros() const
   {
      return m_currentMicros;
   }

   u32 Timer::getDeltaMicros() const
   {
      return m_deltaMicros;
   }

   f32 Timer::getDeltaSeconds() const
   {
      return m_deltaSeconds;
   }

   void Timer::reset(u32 micros)
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
}