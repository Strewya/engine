#pragma once
/********************************************
*  contents:   Utility defines and functions
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
#include "utility/utility.h"
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
      stream << "Memory usage for allocator '" << a.tag << "':" << logLine;
      stream << "   Total memory: " << a.size << logLine;
      stream << "   Max used: " << B << " kB / " << kB << " kB / " << MB << " MB";
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





   //not an allocator, but a helper class that operates on some memory, but is not in charge of doing actual allocations
   struct Freelist
   {
      Freelist* next;

      template<typename T>
      void init(T* start, u32 slots)
      {
         static_assert(sizeof(T) >= sizeof(Freelist), "Using a freelist for a type too small.");

         Freelist* runner = this;
         for( u32 i = 0; i < slots; ++i )
         {
            runner->next = (Freelist*)&start[i];
            runner = runner->next;
         }
         runner->next = nullptr;
      }
   };

   inline u8* allocate(Freelist& a)
   {
      if( a.next == nullptr )
      {
         return nullptr;
      }
      Freelist* head = a.next;
      a.next = head->next;
      return (u8*)head;
   }

   inline void release(Freelist& a, void* ptr)
   {
      Freelist* head = (Freelist*)ptr;
      head->next = a.next;
      a.next = head;
   }
}
