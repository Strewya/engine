//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "utility/color.h"
/******* c++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace core
{
   Color::Color(f32 r, f32 g, f32 b, f32 a)
      : r(r), g(g), b(b), a(a)
   {}

   void Color::clip(f32 threshold)
   {
      if( r < threshold ) r = 0;
      if( g < threshold ) g = 0;
      if( b < threshold ) b = 0;
      if( a < threshold ) a = 0;
   }

   bool operator==(Color l, Color r)
   {
      auto result = 
         (u8)(l.r * 255) == (u8)(r.r * 255) &&
         (u8)(l.g * 255) == (u8)(r.g * 255) &&
         (u8)(l.b * 255) == (u8)(r.b * 255) &&
         (u8)(l.a * 255) == (u8)(r.a * 255);
      return result;
   }

   bool operator!=(Color l, Color r)
   {
      auto result = !(l == r);
      return result;
   }

   Color colorFromHex(u32 hexValue)
   {
      u8 r = hexValue >> 16 & 0xff;
      u8 g = hexValue >> 8 & 0xff;
      u8 b = hexValue >> 0 & 0xff;

      Color result{r / 255.0f, g / 255.0f, b / 255.0f, 1};
      return result;
   }
}

