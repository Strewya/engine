//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "utility/transform.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "utility/geometry/vec2.h"
/******* end headers *******/

namespace core
{
   Transform::Transform(Vec2 p, Vec2 s, float r)
      : position(p), scale(s), rotation(r)
   {
   }

   void Transform::set(Vec2 p, Vec2 s, float r)
   {
      position = p;
      scale = s;
      rotation = r;
   }

   void Transform::translate(Vec2 delta)
   {
      position += delta;
   }

   void Transform::rescale(float uniform)
   {
      scale *= uniform;
   }

   void Transform::rescale(Vec2 nonUniform)
   {
      scale *= nonUniform;
   }

   void Transform::rotate(float degrees)
   {
      rotation += degrees;
   }
}