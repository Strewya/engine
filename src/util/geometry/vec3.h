#pragma once
/********************************************
*  contents:   3D vector function implementations
*  usage:
********************************************/
/******* c++ headers *******/
#include <ostream>
/******* common headers *******/
/******* extra headers *******/
#include "util/geometry/vec_types.h"
/******* end header inclusion *******/

namespace core
{
   namespace vec3
   {
      bool isZero(Vec3 v);
      float length(Vec3 v);
      float length2(Vec3 v);
      Vec3 normalize(Vec3 v);
      float dotProduct(Vec3 l, Vec3 r);
   }

   Vec3 operator-(Vec3 v);

   Vec3& operator+=(Vec3& v, float s);
   Vec3& operator-=(Vec3& v, float s);
   Vec3& operator*=(Vec3& v, float s);
   Vec3& operator/=(Vec3& v, float s);

   Vec3& operator+=(Vec3& l, Vec3 r);
   Vec3& operator-=(Vec3& l, Vec3 r);
   Vec3& operator*=(Vec3& l, Vec3 r);
   Vec3& operator/=(Vec3& l, Vec3 r);

   Vec3 operator*(Vec3 v, float s);
   Vec3 operator/(Vec3 v, float s);

   Vec3 operator*(float s, Vec3 v);
   Vec3 operator/(float s, Vec3 v);

   Vec3 operator+(Vec3 l, Vec3 r);
   Vec3 operator-(Vec3 l, Vec3 r);
   Vec3 operator*(Vec3 l, Vec3 r);
   Vec3 operator/(Vec3 l, Vec3 r);

   bool operator==(Vec3 l, Vec3 r);
   bool operator!=(Vec3 l, Vec3 r);

   std::ostream& operator<<(std::ostream& os, Vec3 v);
}
