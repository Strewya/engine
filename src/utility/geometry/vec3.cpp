//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "utility/geometry/vec3.h"
/******* c++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace core
{
   namespace vec3
   {
      bool isZero(Vec3 v)
      {
         auto result = (v.x == 0.0f && v.y == 0.0f && v.z == 0.0f);
         return result;
      }

      float length(Vec3 v)
      {
         auto result = sqrt(length2(v));
         return result;
      }

      float length2(Vec3 v)
      {
         auto result = v.x*v.x + v.y*v.y + v.z*v.z;
         return result;
      }

      Vec3 normalize(Vec3 v)
      {
         Vec3 result{0, 0, 0};
         auto l2 = length2(v);
         if( l2 != 0 )
         {
            auto l = sqrt(l2);
            result = {v.x / l, v.y / l, v.z / l};
         }
         return result;
      }

      float dotProduct(Vec3 l, Vec3 r)
      {
         auto result = l.x*r.x + l.y*r.y + l.z*r.z;
         return result;
      }
   }

   Vec3& operator+=(Vec3& v, float s)
   {
      v.x += s;
      v.y += s;
      v.z += s;
      return v;
   }

   Vec3& operator-=(Vec3& v, float s)
   {
      v.x -= s;
      v.y -= s;
      v.z -= s;
      return v;
   }

   Vec3& operator*=(Vec3& v, float s)
   {
      v.x *= s;
      v.y *= s;
      v.z *= s;
      return v;
   }

   Vec3& operator/=(Vec3& v, float s)
   {
      v.x /= s;
      v.y /= s;
      v.z /= s;
      return v;
   }

   Vec3& operator+=(Vec3& l, Vec3 r)
   {
      l.x += r.x;
      l.y += r.y;
      l.z += r.z;
      return l;
   }

   Vec3& operator-=(Vec3& l, Vec3 r)
   {
      l.x -= r.x;
      l.y -= r.y;
      l.z -= r.z;
      return l;
   }

   Vec3& operator*=(Vec3& l, Vec3 r)
   {
      l.x *= r.x;
      l.y *= r.y;
      l.z *= r.z;
      return l;
   }

   Vec3& operator/=(Vec3& l, Vec3 r)
   {
      l.x /= r.x;
      l.y /= r.y;
      l.z /= r.z;
      return l;
   }

   Vec3 operator-(Vec3 v)
   {
      Vec3 result{-v.x, -v.y, -v.z};
      return result;
   }

   Vec3 operator+(Vec3 v, float s)
   {
      v += s;
      return v;
   }
   Vec3 operator-(Vec3 v, float s)
   {
      v -= s;
      return v;
   }
   Vec3 operator*(Vec3 v, float s)
   {
      v *= s;
      return v;
   }
   Vec3 operator/(Vec3 v, float s)
   {
      v /= s;
      return v;
   }

   Vec3 operator+(Vec3 l, Vec3 r)
   {
      l += r;
      return l;
   }
   Vec3 operator-(Vec3 l, Vec3 r)
   {
      l -= r;
      return l;
   }
   Vec3 operator*(Vec3 l, Vec3 r)
   {
      l *= r;
      return l;
   }
   Vec3 operator/(Vec3 l, Vec3 r)
   {
      l /= r;
      return l;
   }

   bool operator==(Vec3 l, Vec3 r)
   {
      auto result = l.x == r.x && l.y == r.y && l.z == r.z;
      return result;
   }
   bool operator!=(Vec3 l, Vec3 r)
   {
      auto result = !(l == r);
      return result;
   }

   std::ostream& operator<<(std::ostream& os, Vec3 v)
   {
      os << "{" << v.x << "," << v.y << "," << v.z << "}";
      return os;
   }
}
