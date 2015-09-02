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

   struct LinearAllocator
   {
      u8* memory;
      u32 size;
   };

   struct StackAllocator
   {
      u8* memory;
   };


   inline u8* allocate(LinearAllocator& a, u32 size, u32 align)
   {
      u32 alignBytes = ((u32)a.memory % align);
      u32 totalSize = size + alignBytes;
      u8* result = nullptr;
      if( totalSize <= a.size )
      {
         result = a.memory + alignBytes;
         a.size -= totalSize;
         a.memory += totalSize;
      }
      return result;
   }

   template<typename T>
   inline T* allocate(LinearAllocator& a, u32 count = 1)
   {
      u8* result = allocate(a, sizeof(T)*count, __alignof(T));
      return (T*)result;
   }
}
