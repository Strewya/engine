//headers should be ordered alphabetically!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <util/geometry/circle.h>
/******* c++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace Core
{
   void Circle::set(float cx, float cy, float r)
   {
      center.set(cx, cy);
      radius = r;
   }
}
