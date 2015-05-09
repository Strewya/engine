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

namespace core
{
   template<typename T>
   struct vec2;

   typedef vec2<float> vec2f;
   typedef vec2<int32_t> vec2i;
}
