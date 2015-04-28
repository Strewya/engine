#pragma once
/********************************************
*  contents:   #todo
*  usage:
********************************************/
/******* c++ headers *******/
#include <cstdint>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   inline uint32_t secondsToMicros(uint32_t s)
   {
      return s * 1000U * 1000U;
   }

   inline uint32_t secondsToMicros(float s)
   {
      return static_cast<uint32_t>(s*1000.0f*1000.0f);
   }

   inline uint32_t secondsToMilis(uint32_t s)
   {
      return s * 1000U;
   }

   inline uint32_t secondsToMilis(float s)
   {
      return static_cast<uint32_t>(s*1000.0f);
   }

   inline uint32_t milisToMicros(uint32_t ms)
   {
      return ms * 1000U;
   }

   inline float milisToSeconds(uint32_t ms)
   {
      return static_cast<float>(ms)*0.001f;
   }

   inline float microsToSeconds(uint32_t us)
   {
      return static_cast<float>(us)*0.001f*0.001f;
   }
}
