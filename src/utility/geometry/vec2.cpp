//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "utility/geometry/vec2.h"
/******* c++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace vec2
{
   bool isZero(v2 v)
   {
      auto result = v.x == 0.0f && v.y == 0.0f;
      return result;
   }

   f32 length(v2 v)
   {
      auto result = sqrt(length2(v));
      return result;
   }

   f32 length2(v2 v)
   {
      auto result = dotProduct(v, v);
      return result;
   }

   f32 dotProduct(v2 l, v2 r)
   {
      auto result = l.x*r.x + l.y*r.y;
      return result;
   }

   v2 normalize(v2 v)
   {
      v2 result{0, 0};
      auto l2 = length2(v);
      if( l2 != 0 )
      {
         auto l = sqrt(l2);
         result = {v.x / l, v.y / l};
      }
      return result;
   }

   v2 projection(v2 v, v2 target)
   {
      auto dpv = dotProduct(v, target);
      auto dpt = dotProduct(target, target);
      auto result = (dpv / dpt)*target;
      return result;
   }

   v2 setLength(v2 v, f32 len)
   {
      auto result = normalize(v)*len;
      return result;
   }

   v2 absolute(v2 v)
   {
      if( v.x < 0 ) { v.x = -v.x; }
      if( v.y < 0 ) { v.y = -v.y; }
      return v;
   }
}

v2& operator+=(v2& t, f32 s)
{
   t.x += s;
   t.y += s;
   return t;
}

v2& operator-=(v2& t, f32 s)
{
   t.x -= s;
   t.y -= s;
   return t;
}

v2& operator*=(v2& t, f32 s)
{
   t.x *= s;
   t.y *= s;
   return t;
}

v2& operator/=(v2& t, f32 s)
{
   t.x /= s;
   t.y /= s;
   return t;
}

v2& operator+=(v2& t, v2 v)
{
   t.x += v.x;
   t.y += v.y;
   return t;
}

v2& operator-=(v2& t, v2 v)
{
   t.x -= v.x;
   t.y -= v.y;
   return t;
}

v2& operator*=(v2& t, v2 v)
{
   t.x *= v.x;
   t.y *= v.y;
   return t;
}

v2& operator/=(v2& t, v2 v)
{
   t.x /= v.x;
   t.y /= v.y;
   return t;
}

v2 operator-(v2 v)
{
   v2 result{-v.x, -v.y};
   return result;
}

v2 operator+(v2 v, f32 s)
{
   v += s;
   return v;
}
v2 operator-(v2 v, f32 s)
{
   v -= s;
   return v;
}
v2 operator*(v2 v, f32 s)
{
   v *= s;
   return v;
}
v2 operator/(v2 v, f32 s)
{
   v /= s;
   return v;
}
v2 operator*(f32 s, v2 v)
{
   return v*s;
}
v2 operator/(f32 s, v2 v)
{
   return v / s;
}

v2 operator+(v2 l, v2 r)
{
   l += r;
   return l;
}
v2 operator-(v2 l, v2 r)
{
   l -= r;
   return l;
}
v2 operator*(v2 l, v2 r)
{
   l *= r;
   return l;
}
v2 operator/(v2 l, v2 r)
{
   l /= r;
   return l;
}

bool operator==(v2 l, v2 r)
{
   auto result = l.x == r.x && l.y == r.y;
   return result;
}
bool operator!=(v2 l, v2 r)
{
   auto result = !(l == r);
   return result;
}

bool operator<(v2 l, v2 r)
{
   auto result = (l.x < r.x) && (l.y < r.y);
   return result;
}

std::ostream& operator<<(std::ostream& os, v2 v)
{
   os << "{" << v.x << "," << v.y << "}";
   return os;
}

