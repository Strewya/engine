#pragma once
/********************************************
*  contents:   Forward declaration for vector
*  usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   template<typename T>
   class Vec3;

   typedef Vec3<float> Vec3f;
   typedef Vec3<int32_t> Vec3i;
}
