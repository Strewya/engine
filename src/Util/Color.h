#pragma once
/********************************************
*  class:   Color
*  usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   class Color
   {
   public:
      float r;
      float g;
      float b;
      float a;

      Color();
      Color(float r, float g, float b, float a = 1);

      void set(float r, float g, float b, float a = 1);
      void clip(float threshold);
   };

   bool operator==(Color l, Color r);
   bool operator!=(Color l, Color r);
}
