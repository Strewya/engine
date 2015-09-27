#pragma once
/********************************************
*  contents:   function to load a file into byte array
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   struct Allocator;

   struct LoadedFile
   {
      void* memory;
      u32 size;
   };

   LoadedFile loadFile(const char* filename, Allocator& a);
}
