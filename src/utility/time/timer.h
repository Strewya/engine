#pragma once
/********************************************
*  contents:   ordinary timer class, ever increasing, should store relative time only
*  usage:   max resolution is 2^32 micros == 71min
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   struct Timer
   {
   public:
      Timer();

      void advanceTimeBy(u32 deltaMicros);

      void setTimeScale(f32 timeScale);
      f32 getTimeScale() const;

      u32 getCurrentMicros() const;
      u32 getDeltaMicros() const;
      f32 getDeltaSeconds() const;

      void reset(u32 micros = 0);

   private:
      u32 m_currentMicros;
      u32 m_deltaMicros;
      f32 m_deltaSeconds;
      f32 m_timeScale;
   };
}
