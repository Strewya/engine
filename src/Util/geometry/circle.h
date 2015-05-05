#pragma once
/********************************************
*  contents:   circle type
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include <util/geometry/vec2.h>
/******* end header inclusion *******/

namespace Core
{
   struct Circle
   {
      vec2f center;
      float radius;

      void set(float cx, float cy, float r);
   };
}
