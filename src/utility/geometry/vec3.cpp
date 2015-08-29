//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "utility/geometry/vec3.h"
/******* c++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace vec3
{
   bool isZero(v3 v)
   {
      auto result = (v.x == 0.0f && v.y == 0.0f && v.z == 0.0f);
      return result;
   }

   f32 length(v3 v)
   {
      auto result = sqrt(length2(v));
      return result;
   }

   f32 length2(v3 v)
   {
      auto result = v.x*v.x + v.y*v.y + v.z*v.z;
      return result;
   }

   v3 normalize(v3 v)
   {
      v3 result{0, 0, 0};
      auto l2 = length2(v);
      if( l2 != 0 )
      {
         auto l = sqrt(l2);
         result = {v.x / l, v.y / l, v.z / l};
      }
      return result;
   }

   f32 dotProduct(v3 l, v3 r)
   {
      auto result = l.x*r.x + l.y*r.y + l.z*r.z;
      return result;
   }
}

v3& operator+=(v3& v, f32 s)
{
   v.x += s;
   v.y += s;
   v.z += s;
   return v;
}

v3& operator-=(v3& v, f32 s)
{
   v.x -= s;
   v.y -= s;
   v.z -= s;
   return v;
}

v3& operator*=(v3& v, f32 s)
{
   v.x *= s;
   v.y *= s;
   v.z *= s;
   return v;
}

v3& operator/=(v3& v, f32 s)
{
   v.x /= s;
   v.y /= s;
   v.z /= s;
   return v;
}

v3& operator+=(v3& l, v3 r)
{
   l.x += r.x;
   l.y += r.y;
   l.z += r.z;
   return l;
}

v3& operator-=(v3& l, v3 r)
{
   l.x -= r.x;
   l.y -= r.y;
   l.z -= r.z;
   return l;
}

v3& operator*=(v3& l, v3 r)
{
   l.x *= r.x;
   l.y *= r.y;
   l.z *= r.z;
   return l;
}

v3& operator/=(v3& l, v3 r)
{
   l.x /= r.x;
   l.y /= r.y;
   l.z /= r.z;
   return l;
}

v3 operator-(v3 v)
{
   v3 result{-v.x, -v.y, -v.z};
   return result;
}

v3 operator+(v3 v, f32 s)
{
   v += s;
   return v;
}
v3 operator-(v3 v, f32 s)
{
   v -= s;
   return v;
}
v3 operator*(v3 v, f32 s)
{
   v *= s;
   return v;
}
v3 operator/(v3 v, f32 s)
{
   v /= s;
   return v;
}

v3 operator+(v3 l, v3 r)
{
   l += r;
   return l;
}
v3 operator-(v3 l, v3 r)
{
   l -= r;
   return l;
}
v3 operator*(v3 l, v3 r)
{
   l *= r;
   return l;
}
v3 operator/(v3 l, v3 r)
{
   l /= r;
   return l;
}

bool operator==(v3 l, v3 r)
{
   auto result = l.x == r.x && l.y == r.y && l.z == r.z;
   return result;
}
bool operator!=(v3 l, v3 r)
{
   auto result = !(l == r);
   return result;
}
bool operator<(v3 l, v3 r)
{
   auto result = (l.x < r.x) && (l.y < r.y) && (l.z < r.z);
   return result;
}

std::ostream& operator<<(std::ostream& os, v3 v)
{
   os << "{" << v.x << "," << v.y << "," << v.z << "}";
   return os;
}

