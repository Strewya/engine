//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "utility/time/periodic_timer.h"
/******* c++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace core
{
   PeriodicTimer::PeriodicTimer()
      : PeriodicTimer(0)
   {
   }

   PeriodicTimer::PeriodicTimer(u32 periodMicros)
      : m_periodMicros(periodMicros)
   {
   }

   void PeriodicTimer::advanceTimeBy(u32 deltaMicros)
   {
      m_timer.advanceTimeBy(deltaMicros);
   }
   bool PeriodicTimer::hasElapsed() const
   {
      return m_timer.getCurrentMicros() >= m_periodMicros;
   }
   u32 PeriodicTimer::getRemainingMicros() const
   {
      return m_periodMicros - m_timer.getCurrentMicros();
   }
   f32 PeriodicTimer::getPercentDone() const
   {
      return static_cast<f32>(m_timer.getCurrentMicros()) / static_cast<f32>(m_periodMicros);
   }
   void PeriodicTimer::period()
   {
      m_timer.reset(m_periodMicros);
   }
   void PeriodicTimer::hardReset()
   {
      m_timer.reset();
   }

   u32 PeriodicTimer::getPeriodMicros() const
   {
      return m_periodMicros;
   }
   void PeriodicTimer::setPeriodMicros(u32 periodMicros)
   {
      m_periodMicros = periodMicros;
   }
   const Timer& PeriodicTimer::getTimer() const
   {
      return m_timer;
   }
}