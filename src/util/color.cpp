//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "util/color.h"
/******* c++ headers *******/
#include <cstdint>
/******* extra headers *******/
/******* end headers *******/

namespace core
{
   Color::Color(float r, float g, float b, float a)
      : r(r), g(g), b(b), a(a)
   {}

   void Color::clip(float threshold)
   {
      if( r < threshold ) r = 0;
      if( g < threshold ) g = 0;
      if( b < threshold ) b = 0;
      if( a < threshold ) a = 0;
   }

   bool operator==(Color l, Color r)
   {
      auto result = 
         (uint8_t)(l.r * 255) == (uint8_t)(r.r * 255) &&
         (uint8_t)(l.g * 255) == (uint8_t)(r.g * 255) &&
         (uint8_t)(l.b * 255) == (uint8_t)(r.b * 255) &&
         (uint8_t)(l.a * 255) == (uint8_t)(r.a * 255);
      return result;
   }

   bool operator!=(Color l, Color r)
   {
      auto result = !(l == r);
      return result;
   }

   Color colorFromHex(uint32_t hexValue)
   {
      uint8_t r = hexValue >> 16 & 0xff;
      uint8_t g = hexValue >> 8 & 0xff;
      uint8_t b = hexValue >> 0 & 0xff;

      Color result{r / 255.0f, g / 255.0f, b / 255.0f, 1};
      return result;
   }
}

