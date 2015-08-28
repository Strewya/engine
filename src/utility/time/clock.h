#pragma once
/********************************************
*  contents:   clock type
*  usage:   main time passage calculator
********************************************/
/******* c++ headers *******/
#include <cstdint>
/******* common headers *******/
/******* extra headers *******/
#include "utility/types.h"
/******* end header inclusion *******/

namespace core
{
   struct Clock
   {
   public:
      Clock();

      void update();
      void advanceTimeBy(u64 deltaMicros);

      u32 getFixedStepUpdateCount(u64 frameTime, float& ratio, u64& remainderTime);

      core_class_scope u64 getRealTimeMicros();
      u64 getLastRealTimeMicros() const;

      u64 getCurrentMicros() const;
      u32 getDeltaMicros() const;
      float getDeltaSeconds() const;

      void setTimeScale(double timeScale);
      double getTimeScale() const;

   private:
      u64 m_oldRealTime;
      double m_timeScale;
      u64 m_lastMicros;
      u64 m_currentMicros;
      u64 m_deltaMicros;
      float m_deltaSeconds;
   };
}
