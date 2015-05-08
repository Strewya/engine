#pragma once
/********************************************
*  contents:   transform type that bundles translation, rotation and scale
*  usage:   translation is defined to be from 0,0 dependant on the game
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include "util/geometry/vec2.h"
/******* end header inclusion *******/

namespace core
{
   struct Transform
   {
      Transform(vec2f p = {0, 0}, vec2f s = {1, 1}, float r = 0);

      void set(vec2f p, vec2f s, float r);

      void translate(vec2f delta);

      void rescale(float uniform);
      void rescale(vec2f nonUniform);

      void rotate(float degrees);

      vec2f position;
      vec2f scale;
      float rotation;
   };
}