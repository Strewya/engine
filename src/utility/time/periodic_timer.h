#pragma once
/********************************************
*  contents:   periodic timer, never stops, can be queried if elapsed
*  usage:   should be used for things that have to occur every x time periods
********************************************/
/******* c++ headers *******/
#include <string>
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
#include "utility/time/timer.h"
/******* end header inclusion *******/

namespace core
{
   struct PeriodicTimer
   {
   public:
      PeriodicTimer();
      PeriodicTimer(u32 periodMicros);

      void advanceTimeBy(u32 deltaMicros);
      bool hasElapsed() const;
      u32 getRemainingMicros() const;
      f32 getPercentDone() const;
      void period();
      void hardReset();

      u32 getPeriodMicros() const;
      void setPeriodMicros(u32 periodMicros);
      const Timer& getTimer() const;

   private:
      Timer m_timer;
      u32 m_periodMicros;
   };
}
