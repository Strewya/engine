#pragma once
/********************************************
*  class:   PeriodicTimer
*  usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <string>
/******* common headers *******/
/******* extra headers *******/
#include <Util/Timer.h>
/******* end header inclusion *******/

namespace Core
{
   class PeriodicTimer
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

   protected:
      Timer m_timer;
      uint32_t m_periodMicros;
   };
}
