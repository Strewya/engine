//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Util/Rect.h>
/******* C++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace Core
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

   void Rect::set(float cx, float cy, float hw, float hh)
   {
      center.set(cx, cy);
      halfSize.set(hw, hh);
   }
}
