//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "utility/time/cooldown_timer.h"
/******* c++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace core
{
   CooldownTimer::CooldownTimer()
      : CooldownTimer(0)
   {
   }

   CooldownTimer::CooldownTimer(u32 durationMicros)
      : m_durationMicros(durationMicros)
   {
   }

   void CooldownTimer::advanceTimeBy(u32 deltaMicros)
   {
      m_timer.advanceTimeBy(deltaMicros);
   }
   bool CooldownTimer::hasElapsed() const
   {
      return m_timer.getCurrentMicros() >= m_durationMicros;
   }
   u32 CooldownTimer::getRemainingMicros() const
   {
      if( m_durationMicros < m_timer.getCurrentMicros() )
      {
         return 0;
      }
      return m_durationMicros - m_timer.getCurrentMicros();
   }
   f32 CooldownTimer::getPercentDone() const
   {
      return static_cast<f32>(m_timer.getCurrentMicros()) / static_cast<f32>(m_durationMicros);
   }
   void CooldownTimer::reset()
   {
      m_timer.reset();
   }

   u32 CooldownTimer::getDurationMicros() const
   {
      return m_durationMicros;
   }
   void CooldownTimer::setDurationMicros(u32 durationMicros)
   {
      m_durationMicros = durationMicros;
   }
   const Timer& CooldownTimer::getTimer() const
   {
      return m_timer;
   }
}