#pragma once
/********************************************
*  contents:   Utility defines and functions
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
/******* end header inclusion *******/

#define Bytes(n) (n)
#define Kilobytes(n) ((n)*1024)
#define Megabytes(n) (Kilobytes(n)*1024)
#define Gigabytes(n) (Megabytes(n)*1024)
#define Terabytes(n) (Gigabytes(n)*1024)

namespace core
{
   struct Memory
   {
      u8* ptr;
      u32 size;
   };

   struct MainAllocator
   {
      MainAllocator(Memory m) : memory{m.ptr}, sizeLeft{m.size} {}

      u8* memory;
      u32 sizeLeft;
   };

   u8* alloc(MainAllocator& a, u32 size, u32 align)
   {
      u8* result = nullptr;
      if( size + align < a.sizeLeft )
      {

      }
      return result;
   }
}
