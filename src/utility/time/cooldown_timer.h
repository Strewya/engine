#pragma once
/********************************************
*  contents:   cooldown timer, stops after reaching target duration and must be manually reset
*  usage:
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
   struct CooldownTimer
   {
   public:
      CooldownTimer();
      CooldownTimer(u32 durationMicros);

      void advanceTimeBy(u32 deltaMicros);
      bool hasElapsed() const;
      u32 getRemainingMicros() const;
      f32 getPercentDone() const;
      void reset();

      u32 getDurationMicros() const;
      void setDurationMicros(u32 durationMicros);
      const Timer& getTimer() const;

   private:
      Timer m_timer;
      u32 m_durationMicros;
   };
}
