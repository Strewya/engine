#pragma once
/********************************************
*  contents:   color type class
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   struct Color
   {
      f32 r;
      f32 g;
      f32 b;
      f32 a;

      Color(f32 r = 1, f32 g = 1, f32 b = 1, f32 a = 1);

      void clip(f32 threshold);
   };

   bool operator==(Color l, Color r);
   bool operator!=(Color l, Color r);
   
   Color colorFromHex(u32 hexValue);
}
