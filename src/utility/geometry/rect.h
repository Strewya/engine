#pragma once
/********************************************
*  contents:   Rectangle type declaration
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   struct Rect
   {
      v2 center;
      v2 halfSize;

      f32 left() const;
      f32 right() const;
      f32 top() const;
      f32 bottom() const;

      f32 width() const;
      f32 height() const;

      void set(f32 cx, f32 cy, f32 hw, f32 hh);
   };
}
