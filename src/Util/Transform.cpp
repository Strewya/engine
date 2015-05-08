//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "util/transform.h"
/******* c++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace core
{
   Transform::Transform(vec2f p, vec2f s, float r)
      : position(p), scale(s), rotation(r)
   {
   }

   void Transform::set(vec2f p, vec2f s, float r)
   {
      position = p;
      scale = s;
      rotation = r;
   }

   void Transform::translate(vec2f delta)
   {
      position += delta;
   }

   void Transform::rescale(float uniform)
   {
      scale *= uniform;
   }

   void Transform::rescale(vec2f nonUniform)
   {
      scale *= nonUniform;
   }

   void Transform::rotate(float degrees)
   {
      rotation += degrees;
   }
}