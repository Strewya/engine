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
   Transform::Transform(v2 p, v2 s, f32 r)
      : position(p), scale(s), rotation(r)
   {
   }

   void Transform::set(v2 p, v2 s, f32 r)
   {
      position = p;
      scale = s;
      rotation = r;
   }

   void Transform::translate(v2 delta)
   {
      position += delta;
   }

   void Transform::rescale(f32 uniform)
   {
      scale *= uniform;
   }

   void Transform::rescale(v2 nonUniform)
   {
      scale *= nonUniform;
   }

   void Transform::rotate(f32 degrees)
   {
      rotation += degrees;
   }
}