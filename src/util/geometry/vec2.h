#pragma once
/********************************************
*  contents:   2D vector function declarations
*  usage:
********************************************/
/******* c++ headers *******/
#include <ostream>
/******* common headers *******/
#include "util/geometry/vec_types.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   namespace vec2
   {
      bool isZero(Vec2 v);

      float length(Vec2 v);
      float length2(Vec2 v);
      Vec2 normalize(Vec2 v);
      float dotProduct(Vec2 l, Vec2 r);
      Vec2 projection(Vec2 v, Vec2 target);
   }

   Vec2 operator-(Vec2 v);

   Vec2& operator+=(Vec2& v, float s);
   Vec2& operator-=(Vec2& v, float s);
   Vec2& operator*=(Vec2& v, float s);
   Vec2& operator/=(Vec2& v, float s);

   Vec2& operator+=(Vec2& l, Vec2 r);
   Vec2& operator-=(Vec2& l, Vec2 r);
   Vec2& operator*=(Vec2& l, Vec2 r);
   Vec2& operator/=(Vec2& l, Vec2 r);

   Vec2 operator*(Vec2 v, float s);
   Vec2 operator/(Vec2 v, float s);
   
   Vec2 operator*(float s, Vec2 v);
   Vec2 operator/(float s, Vec2 v);

   Vec2 operator+(Vec2 l, Vec2 r);
   Vec2 operator-(Vec2 l, Vec2 r);
   Vec2 operator*(Vec2 l, Vec2 r);
   Vec2 operator/(Vec2 l, Vec2 r);

   bool operator==(Vec2 l, Vec2 r);
   bool operator!=(Vec2 l, Vec2 r);

   std::ostream& operator<<(std::ostream& os, Vec2 v);
}
