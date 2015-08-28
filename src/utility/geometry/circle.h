#pragma once
/********************************************
*  contents:   circle type
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include "utility/geometry/vec_types.h"
/******* end header inclusion *******/

namespace core
{
   struct Circle
   {
      Vec2 center;
      float radius;
   };
}
