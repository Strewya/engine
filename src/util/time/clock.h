#pragma once
/********************************************
*  contents:   clock type
*  usage:   main time passage calculator
********************************************/
/******* c++ headers *******/
#include <cstdint>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   struct Clock
   {
   public:
      Clock();

      void update();
      void updateBy(uint64_t deltaMicros);

      uint32_t getFixedStepUpdateCount(uint64_t frameTime, float& ratio, uint64_t& remainderTime);

      static uint64_t getRealTimeMicros();
      uint64_t getLastRealTimeMicros() const;

      uint64_t getCurrentMicros() const;
      uint32_t getDeltaMicros() const;
      float getDeltaSeconds() const;

      void setTimeScale(double timeScale);
      double getTimeScale() const;

   private:
      uint64_t m_oldRealTime;
      double m_timeScale;
      uint64_t m_lastMicros;
      uint64_t m_currentMicros;
      uint64_t m_deltaMicros;
      float m_deltaSeconds;
   };
}