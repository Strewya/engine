//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "util/time/timer.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "util/time/time_unit_conversions.h"
/******* end headers *******/

namespace Core
{
   Timer::Timer() :
      m_currentMicros(0),
      m_deltaMicros(0),
      m_deltaSeconds(0),
      m_timeScale(1)
   {
   }

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
}