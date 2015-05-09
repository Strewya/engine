#pragma once
/********************************************
*  contents:   color type class
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   struct Color
   {
      float r;
      float g;
      float b;
      float a;

      Color(float r = 1, float g = 1, float b = 1, float a = 1);

      void set(float r, float g, float b, float a = 1);
      void clip(float threshold);
   };

   bool operator==(Color l, Color r);
   bool operator!=(Color l, Color r);
}
