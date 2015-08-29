#pragma once
/********************************************
*  contents:   3D vector function implementations
*  usage:
********************************************/
/******* c++ headers *******/
#include <ostream>
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace vec3
{
   bool isZero(v3 v);
   f32 length(v3 v);
   f32 length2(v3 v);
   v3 normalize(v3 v);
   f32 dotProduct(v3 l, v3 r);
}

v3 operator-(v3 v);

v3& operator+=(v3& v, f32 s);
v3& operator-=(v3& v, f32 s);
v3& operator*=(v3& v, f32 s);
v3& operator/=(v3& v, f32 s);

v3& operator+=(v3& l, v3 r);
v3& operator-=(v3& l, v3 r);
v3& operator*=(v3& l, v3 r);
v3& operator/=(v3& l, v3 r);

v3 operator*(v3 v, f32 s);
v3 operator/(v3 v, f32 s);

v3 operator*(f32 s, v3 v);
v3 operator/(f32 s, v3 v);

v3 operator+(v3 l, v3 r);
v3 operator-(v3 l, v3 r);
v3 operator*(v3 l, v3 r);
v3 operator/(v3 l, v3 r);

bool operator==(v3 l, v3 r);
bool operator!=(v3 l, v3 r);
bool operator<(v3 l, v3 r);

std::ostream& operator<<(std::ostream& os, v3 v);
