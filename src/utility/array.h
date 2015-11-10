#pragma once
/********************************************
*  contents:   #description
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   template<typename T>
   struct Array
   {
      T* data;
      u32 count;
   };

   template<typename T>
   T* begin(Array<T> array)
   {
      return array.data;
   }

   template<typename T>
   T* end(Array<T> array)
   {
      return array.data + array.count;
   }
}