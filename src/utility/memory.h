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
#define Kilobytes(n) (Bytes(n)*1024ULL)
#define Megabytes(n) (Kilobytes(n)*1024ULL)
#define Gigabytes(n) (Megabytes(n)*1024ULL)
#define Terabytes(n) (Gigabytes(n)*1024ULL)

namespace core
{
   // Fixed memory manager. Only allocations are supported, frees are not.
   struct LinearAllocator
   {
      const char* tag;
      u8* memory;
      u32 size;
      u32 allocated;
   };

   inline std::ostream& operator<<(std::ostream& stream, LinearAllocator& a)
   {
      u32 B = a.allocated;
      f32 kB = B / 1024.0f;
      f32 MB = kB / 1024;
      stream << "Memory usage for allocator '" << a.tag << "':\n\tTotal memory: " << a.size << "\n\tMax used: " << B << " kB / " << kB << " kB / " << MB << " MB";
      return stream;
   }

   inline u8* allocate(LinearAllocator& a, u32 size, u32 align)
   {
      u8* memory = a.memory + a.allocated;
      u32 alignBytes = (u32)((u32)memory % align);
      u32 totalSize = size + alignBytes;
      if( a.allocated + totalSize <= a.size )
      {
         memory += alignBytes;
         a.allocated += totalSize;
         return memory;
      }
      return nullptr;
   }

   template<typename T>
   inline T* allocate(LinearAllocator& a, u32 count = 1)
   {
      u8* result = allocate(a, sizeof(T)*count, __alignof(T));
      return (T*)result;
   }




   struct ScratchAllocator
   {
      u8* memory;
      u32 size;
      u32 allocated;
   };

   inline u8* allocate(ScratchAllocator& a, u32 size, u32 align)
   {
      u8* memory = a.memory + a.allocated;
      u32 alignBytes = (u32)((u64)memory % align);
      u32 totalSize = size + alignBytes;
      if( a.allocated + totalSize <= a.size )
      {
         memory += alignBytes;
         a.allocated += totalSize;
         return memory;
      }
      return nullptr;
   }

   inline void reset(ScratchAllocator& a)
   {
      a.allocated = 0;
   }
}
