#pragma once
/********************************************
*  contents:   transform type that bundles translation, rotation and scale
*  usage:   translation is defined to be from 0,0 dependant on the game
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   struct Transform
   {
      Transform(v2 p = {0, 0}, v2 s = {1, 1}, f32 r = 0);

      void set(v2 p, v2 s, f32 r);

      void translate(v2 delta);

      void rescale(f32 uniform);
      void rescale(v2 nonUniform);

      void rotate(f32 degrees);

      v2 position;
      v2 scale;
      f32 rotation;
   };
}