#pragma once
/********************************************
*  contents:   conversions for time units
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   inline u32 secondsToMicros(u32 s)
   {
      return s * 1000U * 1000U;
   }

   inline u32 secondsToMicros(f32 s)
   {
      return static_cast<u32>(s*1000.0f*1000.0f);
   }

   inline u32 secondsToMilis(u32 s)
   {
      return s * 1000U;
   }

   inline u32 secondsToMilis(f32 s)
   {
      return static_cast<u32>(s*1000.0f);
   }

   inline u32 milisToMicros(u32 ms)
   {
      return ms * 1000U;
   }

   inline f32 milisToSeconds(u32 ms)
   {
      return static_cast<f32>(ms)*0.001f;
   }

   inline f32 microsToSeconds(u32 us)
   {
      return static_cast<f32>(us)*0.001f*0.001f;
   }
}
