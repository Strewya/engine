#pragma once
/********************************************
*  class:   Timer
*  usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   class Timer
   {
   public:
      Timer();

      void updateBy(uint32_t deltaMicros);

      void setTimeScale(float timeScale);
      float getTimeScale() const;

      uint32_t getCurrentMicros() const;
      uint32_t getDeltaMicros() const;
      float getDeltaSeconds() const;

      void reset(uint32_t micros = 0);

   protected:
      uint32_t m_currentMicros;
      uint32_t m_deltaMicros;
      float m_deltaSeconds;
      float m_timeScale;
   };
}
