//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "util/geometry/rect.h"
/******* c++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace core
{
   float Rect::left() const
   {
      return center.x - halfSize.x;
   }

   float Rect::right() const
   {
      return center.x + halfSize.x;
   }

   float Rect::top() const
   {
      return center.y + halfSize.y;
   }

   float Rect::bottom() const
   {
      return center.y - halfSize.y;
   }

   float Rect::width() const
   {
      return halfSize.x * 2;
   }

   float Rect::height() const
   {
      return halfSize.y * 2;
   }

   void Rect::set(float cx, float cy, float hw, float hh)
   {
      center.set(cx, cy);
      halfSize.set(hw, hh);
   }
}
