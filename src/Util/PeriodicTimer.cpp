//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Util/PeriodicTimer.h>
/******* C++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace Core
{
   PeriodicTimer::PeriodicTimer()
      : PeriodicTimer(0)
   {
   }

   PeriodicTimer::PeriodicTimer(uint32_t periodMicros)
      : m_periodMicros(periodMicros)
   {
   }

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
}