#pragma once
/********************************************
*  class:   Vec2
*  usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include <Util/Vec2.h>
/******* end header inclusion *******/

namespace Core
{
   class Rect
   {
   public:
      Vec2f center;
      Vec2f halfSize;

      float left() const;
      float right() const;
      float top() const;
      float bottom() const;

      void set(float cx, float cy, float hw, float hh);
   };
}
