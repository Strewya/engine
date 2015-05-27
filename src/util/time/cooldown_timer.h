#pragma once
/********************************************
*  contents:   cooldown timer, stops after reaching target duration and must be manually reset
*  usage:
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
   struct CooldownTimer
   {
   public:
      CooldownTimer();
      CooldownTimer(uint32_t durationMicros);

      void advanceTimeBy(uint32_t deltaMicros);
      bool hasElapsed() const;
      uint32_t getRemainingMicros() const;
      float getPercentDone() const;
      void reset();

      uint32_t getDurationMicros() const;
      void setDurationMicros(uint32_t durationMicros);
      const Timer& getTimer() const;

   private:
      Timer m_timer;
      uint32_t m_durationMicros;
   };
}
