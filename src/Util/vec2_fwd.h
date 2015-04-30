#pragma once
/********************************************
*  contents:   2D vector type forward declaration
*  usage:
********************************************/
/******* c++ headers *******/
#include <cstdint>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   template<typename T>
   class Vec2;

   typedef Vec2<float> Vec2f;
   typedef Vec2<int32_t> Vec2i;
}
