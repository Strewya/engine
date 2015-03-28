//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Util/Color.h>
/******* C++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace Core
{
   Color::Color(float r, float g, float b, float a)
      : r(r), g(g), b(b), a(a)
   {
   }

   Color::Color()
      : Color(1, 1, 1)
   {
   }

   void Color::set(float r, float g, float b, float a)
   {
      this->r = r;
      this->g = g;
      this->b = b;
      this->a = a;
   }

   void Color::clip(float threshold)
   {
      if( r < threshold ) r = 0;
      if( g < threshold ) g = 0;
      if( b < threshold ) b = 0;
      if( a < threshold ) a = 0;
   }

   bool operator==(Color l, Color r)
   {
      return
         (uint8_t)(l.r * 255) == (uint8_t)(r.r * 255) &&
         (uint8_t)(l.g * 255) == (uint8_t)(r.g * 255) &&
         (uint8_t)(l.b * 255) == (uint8_t)(r.b * 255) &&
         (uint8_t)(l.a * 255) == (uint8_t)(r.a * 255);
   }
   
   bool operator!=(Color l, Color r)
   {
      return !(l == r);
   }
}

