#pragma once
/********************************************
*  contents:   2D vector function declarations
*  usage:
********************************************/
/******* c++ headers *******/
#include <ostream>
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace vec2
{
   bool isZero(v2 v);

   f32 length(v2 v);
   f32 length2(v2 v);
   f32 dotProduct(v2 l, v2 r);
   v2 normalize(v2 v);
   v2 projection(v2 v, v2 target);
   v2 setLength(v2 v, f32 len);
   v2 absolute(v2 v);
}

v2 operator-(v2 v);

v2& operator+=(v2& v, f32 s);
v2& operator-=(v2& v, f32 s);
v2& operator*=(v2& v, f32 s);
v2& operator/=(v2& v, f32 s);

v2& operator+=(v2& l, v2 r);
v2& operator-=(v2& l, v2 r);
v2& operator*=(v2& l, v2 r);
v2& operator/=(v2& l, v2 r);

v2 operator*(v2 v, f32 s);
v2 operator/(v2 v, f32 s);

v2 operator*(f32 s, v2 v);
v2 operator/(f32 s, v2 v);

v2 operator+(v2 l, v2 r);
v2 operator-(v2 l, v2 r);
v2 operator*(v2 l, v2 r);
v2 operator/(v2 l, v2 r);

bool operator==(v2 l, v2 r);
bool operator!=(v2 l, v2 r);
bool operator<(v2 l, v2 r);

std::ostream& operator<<(std::ostream& os, v2 v);

