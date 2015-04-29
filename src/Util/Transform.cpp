//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "util/transform.h"
/******* c++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace Core
{
   Transform::Transform(Vec2f p, Vec2f s, float r)
      : position(p), scale(s), rotation(r)
   {
   }

   void Transform::set(Vec2f p, Vec2f s, float r)
   {
      position = p;
      scale = s;
      rotation = r;
   }

   void Transform::translate(Vec2f delta)
   {
      position += delta;
   }

   void Transform::rescale(float uniform)
   {
      scale *= uniform;
   }

   void Transform::rescale(Vec2f nonUniform)
   {
      scale *= nonUniform;
   }

   void Transform::rotate(float degrees)
   {
      rotation += degrees;
   }
}