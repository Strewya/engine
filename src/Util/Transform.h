#pragma once
/********************************************
*	class:	Transform
*	usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include <Util/Vec2.h>
/******* end header inclusion *******/

namespace Core
{
   class Transform
   {
   public:
      Vec2f position;
      Vec2f scale;
      float rotation;

      Transform();
      Transform(Vec2f p, Vec2f s, float r);
      Transform(Vec2f p, float s, float r);

      void set(Vec2f p, Vec2f s, float r);
      void set(Vec2f p, float s, float r);
   };
}