//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "util/geometry/vec2.h"
/******* c++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace core
{
   namespace vec2
   {
      bool isZero(Vec2 v)
      {
         auto result = v.x == 0.0f && v.y == 0.0f;
         return result;
      }

      float length(Vec2 v)
      {
         auto result = sqrt(length2(v));
         return result;
      }

      float length2(Vec2 v)
      {
         auto result = v.x*v.x + v.y*v.y;
         return result;
      }

      float dotProduct(Vec2 l, Vec2 r)
      {
         auto result = l.x*r.x + l.y*r.y;
         return result;
      }

      Vec2 normalize(Vec2 v)
      {
         Vec2 result{0, 0};
         auto l2 = length2(v);
         if( l2 != 0 )
         {
            auto l = sqrt(l2);
            result = {v.x / l, v.y / l};
         }
         return result;
      }

      Vec2 projection(Vec2 v, Vec2 target)
      {
         auto dpv = dotProduct(v, target);
         auto dpt = dotProduct(target, target);
         auto result = (dpv / dpt)*target;
         return result;
      }

      Vec2 setLength(Vec2 v, float len)
      {
         return normalize(v)*len;
      }
   }

   Vec2& operator+=(Vec2& t, float s)
   {
      t.x += s;
      t.y += s;
      return t;
   }

   Vec2& operator-=(Vec2& t, float s)
   {
      t.x -= s;
      t.y -= s;
      return t;
   }

   Vec2& operator*=(Vec2& t, float s)
   {
      t.x *= s;
      t.y *= s;
      return t;
   }

   Vec2& operator/=(Vec2& t, float s)
   {
      t.x /= s;
      t.y /= s;
      return t;
   }

   Vec2& operator+=(Vec2& t, Vec2 v)
   {
      t.x += v.x;
      t.y += v.y;
      return t;
   }

   Vec2& operator-=(Vec2& t, Vec2 v)
   {
      t.x -= v.x;
      t.y -= v.y;
      return t;
   }

   Vec2& operator*=(Vec2& t, Vec2 v)
   {
      t.x *= v.x;
      t.y *= v.y;
      return t;
   }

   Vec2& operator/=(Vec2& t, Vec2 v)
   {
      t.x /= v.x;
      t.y /= v.y;
      return t;
   }

   Vec2 operator-(Vec2 v)
   {
      Vec2 result{-v.x, -v.y};
      return result;
   }

   Vec2 operator+(Vec2 v, float s)
   {
      v += s;
      return v;
   }
   Vec2 operator-(Vec2 v, float s)
   {
      v -= s;
      return v;
   }
   Vec2 operator*(Vec2 v, float s)
   {
      v *= s;
      return v;
   }
   Vec2 operator/(Vec2 v, float s)
   {
      v /= s;
      return v;
   }
   Vec2 operator*(float s, Vec2 v)
   {
      return v*s;
   }
   Vec2 operator/(float s, Vec2 v)
   {
      return v / s;
   }

   Vec2 operator+(Vec2 l, Vec2 r)
   {
      l += r;
      return l;
   }
   Vec2 operator-(Vec2 l, Vec2 r)
   {
      l -= r;
      return l;
   }
   Vec2 operator*(Vec2 l, Vec2 r)
   {
      l *= r;
      return l;
   }
   Vec2 operator/(Vec2 l, Vec2 r)
   {
      l /= r;
      return l;
   }

   bool operator==(Vec2 l, Vec2 r)
   {
      auto result = l.x == r.x && l.y == r.y;
      return result;
   }
   bool operator!=(Vec2 l, Vec2 r)
   {
      auto result = !(l == r);
      return result;
   }

   std::ostream& operator<<(std::ostream& os, Vec2 v)
   {
      os << "{" << v.x << "," << v.y << "}";
      return os;
   }
}
