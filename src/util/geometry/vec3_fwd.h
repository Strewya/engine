#pragma once
/********************************************
*  contents:   3D vector type forward declaration
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
   struct vec3;

   typedef vec3<float> vec3f;
   typedef vec3<int32_t> vec3i;
}
