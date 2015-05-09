#pragma once
/********************************************
*  contents:   Rectangle type declaration
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include "util/geometry/vec2.h"
/******* end header inclusion *******/

namespace core
{
   struct Rect
   {
      vec2f center;
      vec2f halfSize;

      float left() const;
      float right() const;
      float top() const;
      float bottom() const;

      void set(float cx, float cy, float hw, float hh);
   };
}
