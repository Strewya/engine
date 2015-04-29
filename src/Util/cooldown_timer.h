#pragma once
/********************************************
*  contents:   #todo
*  usage:
********************************************/
/******* c++ headers *******/
#include <cstdint>
#include <string>
/******* common headers *******/
/******* extra headers *******/
#include "util/timer.h"
/******* end header inclusion *******/

namespace Core
{
   class CooldownTimer
   {
   public:
      CooldownTimer();
      CooldownTimer(uint32_t durationMicros);

      void updateBy(uint32_t deltaMicros);
      bool hasElapsed() const;
      uint32_t getRemainingMicros() const;
      float getPercentDone() const;
      void reset();

      uint32_t getDurationMicros() const;
      void setDurationMicros(uint32_t durationMicros);
      const Timer& getTimer() const;

   protected:
      Timer m_timer;
      uint32_t m_durationMicros;
   };
}
