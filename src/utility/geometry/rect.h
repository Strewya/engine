#pragma once
/********************************************
*  contents:   Rectangle type declaration
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include "utility/geometry/vec_types.h"
/******* end header inclusion *******/

namespace core
{
   struct Rect
   {
      Vec2 center;
      Vec2 halfSize;

      float left() const;
      float right() const;
      float top() const;
      float bottom() const;

      float width() const;
      float height() const;

      void set(float cx, float cy, float hw, float hh);
   };
}
