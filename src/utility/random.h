#pragma once
/********************************************
*  contents:   #description
*  usage:      
********************************************/
/******* c++ headers *******/
#include <random>
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   struct Random
   {
   public:
      Random();
      Random(u32 seed);
      Random(u64 seed);

      //change the seed
      void reseed(u32 seed);
      void reseed(u64 seed);

      //in interval [a, b]
      i32 randInt(i32 a, i32 b);

      //in interval [a, b)
      i32 randIndex(i32 a, i32 b);

      //in interval between 0 and 1
      f32 randFloat();

   private:
      std::mt19937 m_randEngine;
   };
}
