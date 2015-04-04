#pragma once
/********************************************
*  class:   Transform
*  usage:
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
      Transform(Vec2f p = {0, 0}, Vec2f s = {1, 1}, float r = 0);

      void set(Vec2f p, Vec2f s, float r);

      void translate(Vec2f delta);

      void rescale(float uniform);
      void rescale(Vec2f nonUniform);

      void rotate(float degrees);

      Vec2f position;
      Vec2f scale;
      float rotation;
   };
}