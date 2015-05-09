#pragma once
/********************************************
*  contents:   periodic timer, never stops, can be queried if elapsed
*  usage:   should be used for things that have to occur every x time periods
********************************************/
/******* c++ headers *******/
#include <cstdint>
#include <string>
/******* common headers *******/
/******* extra headers *******/
#include "util/time/timer.h"
/******* end header inclusion *******/

namespace core
{
   struct PeriodicTimer
   {
   public:
      PeriodicTimer();
      PeriodicTimer(uint32_t periodMicros);

      void updateBy(uint32_t deltaMicros);
      bool hasElapsed() const;
      uint32_t getRemainingMicros() const;
      float getPercentDone() const;
      void period();
      void hardReset();

      uint32_t getPeriodMicros() const;
      void setPeriodMicros(uint32_t periodMicros);
      const Timer& getTimer() const;

   private:
      Timer m_timer;
      uint32_t m_periodMicros;
   };
}
