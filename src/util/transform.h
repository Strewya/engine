#pragma once
/********************************************
*  contents:   transform type that bundles translation, rotation and scale
*  usage:   translation is defined to be from 0,0 dependant on the game
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include "util/geometry/vec_types.h"
/******* end header inclusion *******/

namespace core
{
   struct Transform
   {
      Transform(Vec2 p = {0, 0}, Vec2 s = {1, 1}, float r = 0);

      void set(Vec2 p, Vec2 s, float r);

      void translate(Vec2 delta);

      void rescale(float uniform);
      void rescale(Vec2 nonUniform);

      void rotate(float degrees);

      Vec2 position;
      Vec2 scale;
      float rotation;
   };
}