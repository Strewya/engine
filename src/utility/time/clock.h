#pragma once
/********************************************
*  contents:   clock type
*  usage:   main time passage calculator
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   struct Clock
   {
   public:
      Clock();

      void update();
      void advanceTimeBy(u64 deltaMicros);

      u32 getFixedStepUpdateCount(u64 frameTime, f32& ratio, u64& remainderTime);

      core_class_scope u64 getRealTimeMicros();
      u64 getLastRealTimeMicros() const;

      u64 getCurrentMicros() const;
      u32 getDeltaMicros() const;
      f32 getDeltaSeconds() const;

      void setTimeScale(f64 timeScale);
      f64 getTimeScale() const;

   private:
      u64 m_oldRealTime;
      f64 m_timeScale;
      u64 m_lastMicros;
      u64 m_currentMicros;
      u64 m_deltaMicros;
      f32 m_deltaSeconds;
   };
}
