#pragma once
/********************************************
*  contents:   Utility defines and functions
*  usage:
********************************************/
/******* c++ headers *******/
#include <iomanip>
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
   struct byteSizes
   {
      u32 m_B;

      byteSizes(u32 B) : m_B(B) {}
   };
   inline std::ostream& operator<<(std::ostream& stream, byteSizes bs)
   {
      u32 B = bs.m_B;
      f32 kB = B / 1024.0f;
      f32 MB = kB / 1024;
      stream << B << " B / " << std::fixed << kB << " kB / " << MB << " MB";
      return stream;
   }

   struct memoryRequirement
   {
      void* m_start;
      void* m_end;

      memoryRequirement(void* start, void* end) : m_start{start}, m_end{end} {}
   };
   inline std::ostream& operator<<(std::ostream& stream, memoryRequirement mr)
   {
      stream << byteSizes(u32((u8*)mr.m_end - (u8*)mr.m_start));
      return stream;
   }

   struct memoryAddress
   {
      void* m_address;

      memoryAddress(void* address) : m_address(address) {}
   };
   inline std::ostream& operator<<(std::ostream& stream, memoryAddress ma)
   {
      stream << "0x" << ma.m_address;
      return stream;
   }

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
      stream << "   Total memory: " << byteSizes(a.size) << logLine;
      stream << "   Currently using: " << byteSizes(a.allocated);
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
