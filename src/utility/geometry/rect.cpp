//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "utility/geometry/rect.h"
/******* c++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace core
{
   f32 Rect::left() const
   {
      return center.x - halfSize.x;
   }

   f32 Rect::right() const
   {
      return center.x + halfSize.x;
   }

   f32 Rect::top() const
   {
      return center.y + halfSize.y;
   }

   f32 Rect::bottom() const
   {
      return center.y - halfSize.y;
   }

   f32 Rect::width() const
   {
      return halfSize.x * 2;
   }

   f32 Rect::height() const
   {
      return halfSize.y * 2;
   }

   void Rect::set(f32 cx, f32 cy, f32 hw, f32 hh)
   {
      center = {cx, cy};
      halfSize = {hw, hh};
   }
}
