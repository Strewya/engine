#pragma once
/********************************************
*	class:	Circle
*	usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include <Util/Vec2.h>
/******* end header inclusion *******/

namespace Core
{
   class Circle
   {
   public:
      Vec2f center;
      float radius;

      Circle();
      Circle(Vec2f c, float r);

      void set(Vec2f c, float r);
      void set(float cx, float cy, float r);
   };
}
