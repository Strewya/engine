//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "util/time/cooldown_timer.h"
/******* c++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace core
{
   CooldownTimer::CooldownTimer()
      : CooldownTimer(0)
   {
   }

   CooldownTimer::CooldownTimer(uint32_t durationMicros)
      : m_durationMicros(durationMicros)
   {
   }

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
      if( m_durationMicros < m_timer.getCurrentMicros() )
      {
         return 0;
      }
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
}