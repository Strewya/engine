//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Util/Transform.h>
/******* C++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace Core
{
   Transform::Transform()
      : Transform({0, 0}, 1, 0)
   {}

   Transform::Transform(Vec2f p, float s, float r)
      : Transform(p, {s, s}, r)
   {}

   Transform::Transform(Vec2f p, Vec2f s, float r)
      : position(p), scale(s), rotation(r)
   {}

   void Transform::set(Vec2f p, Vec2f s, float r)
   {
      position = p;
      scale = s;
      rotation = r;
   }

   void Transform::set(Vec2f p, float s, float r)
   {
      set(p, {s, s}, r);
   }
}