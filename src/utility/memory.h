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

   inline void outputAllocatorData(std::ostream& stream, const char* tag, u32 totalSize, u32 allocated, u32 peakAllocation)
   {
      stream << "Memory usage for allocator '" << tag << "':" << logLine;
      stream << "   Total memory: " << byteSizes(totalSize) << logLine;
      stream << "   Currently using: " << byteSizes(allocated) << logLine;
      stream << "   Peak usage: " << byteSizes(peakAllocation) << logLine;
   }


   // Fixed memory manager. Only allocations are supported, frees are not.
   struct LinearAllocator
   {
      const char* tag;
      u8* memory;
      u32 size;
      u32 allocated;
      u32 maxAllocated;
   };

   template<typename T>
   inline LinearAllocator spawnLinearAllocator(const char* tag, T& parent, u32 size)
   {
      LinearAllocator result{};
      result.tag = tag;
      result.memory = allocate(parent, size, 4);
      result.size = size;
      result.allocated = result.maxAllocated = 0;
      return result;
   }

   inline std::ostream& operator<<(std::ostream& stream, LinearAllocator& a)
   {
      outputAllocatorData(stream, a.tag, a.size, a.allocated, a.maxAllocated);
      return stream;
   }

   inline u8* allocate(LinearAllocator& a, u32 size, u32 align)
   {
      u8* result = a.memory + a.allocated;
      align = align - (u32)result % align;
      size += align;
      if( a.allocated + size <= a.size )
      {
         result += align;
         a.allocated += size;
         return result;
      }
      return nullptr;
   }

   template<typename T>
   inline T* allocate(LinearAllocator& a, u32 count = 1)
   {
      u8* result = allocate(a, sizeof(T)*count, __alignof(T));
      return (T*)result;
   }


   struct StackAllocator
   {
      const char* tag;
      u8* memory;
      u32 size;
      u32 allocated;
      u32 maxAllocated;
   };
   
   template<typename T>
   inline StackAllocator spawnStackAllocator(const char* tag, T& parent, u32 size)
   {
      StackAllocator result{};
      result.memory = allocate(parent, size, 4);
      result.size = size;
      result.tag = tag;
      result.allocated = result.maxAllocated = 0;
      return result;
   }

   inline std::ostream& operator<<(std::ostream& stream, StackAllocator& a)
   {
      outputAllocatorData(stream, a.tag, a.size, a.allocated, a.maxAllocated);
      return stream;
   }

   inline u8* allocate(StackAllocator& a, u32 size, u32 align)
   {
      u8* result = a.memory + a.allocated;
      if( (u32)result % align != 0 )
      {
         align = align - (u32)result % align;
      }
      size += align;
      if( a.allocated + size <= a.size )
      {
         result += align;
         a.allocated += size;
         *(result - 1) = (u8)align;
         return result;
      }
      return nullptr;
   }

   template<typename T>
   inline T* allocate(StackAllocator& a, u32 count = 1)
   {
      u8* result = allocate(a, sizeof(T)*count, __alignof(T));
      return (T*)result;
   }

   inline void deallocate(StackAllocator& a)
   {
      a.allocated = 0;
   }

   inline void deallocate(StackAllocator& a, void* ptr)
   {
      CORE_ASSERT_DBGERR(a.memory <= ptr && ptr < a.memory + a.size, "Attempting to free a pointer not owned by allocator ", a.tag);
      CORE_ASSERT_DBGERR(a.memory <= ptr && ptr < a.memory + a.allocated, "Attempting to free an already free pointer in ", a.tag);
      
      u32 alignment = *((u8*)ptr - 1);
      u32 size = (u32)(((u8*)a.memory + a.allocated) - (u8*)ptr);
      size += alignment;
      a.allocated -= size;
   }


   //not an allocator, but a helper class that operates on some memory, but is not in charge of doing actual allocations
   struct Freelist
   {
      Freelist* next;

      template<typename T>
      void init(T* start, u32 slots)
      {
         static_assert(sizeof(T) >= sizeof(Freelist), "Using the freelist for a type too small.");

         Freelist* runner = this;
         for( u32 i = 0; i < slots; ++i )
         {
            runner->next = (Freelist*)&start[i];
            runner = runner->next;
         }
         runner->next = nullptr;
      }
   };

   inline void* allocate(Freelist& a)
   {
      if( a.next == nullptr )
      {
         return nullptr;
      }
      union
      {
         Freelist* head;
         void* result;
      };
      head = a.next;
      a.next = head->next;
      return result;
   }

   inline void deallocate(Freelist& a, void* ptr)
   {
      union
      {
         Freelist* head;
         void* address;
      };
      address = ptr;
      head->next = a.next;
      a.next = head;
   }

   struct IndexedFreelist
   {
      u8* memory;
      u32 objectSize;
      u32 arraySize;
      u32 next;

      template<typename T>
      void init(T* start, u32 slots)
      {
         static_assert(sizeof(T) >= sizeof(u32), "Using the indexed freelist for a type too small.");
         memory = (u8*)start;
         objectSize = sizeof(T);
         arraySize = slots;
         u32* runner = &next;
         for( auto i = 0; i <= slots; ++i )
         {
            *runner = i;
            runner = (u32*)(start + i);
         }
      }
   };

   inline void* allocate(IndexedFreelist& a)
   {
      if( a.next == a.arraySize )
      {
         return nullptr;
      }
      void* result = a.memory + a.next*a.objectSize;
      a.next = *((u32*)result);
      memset(result, 0, a.objectSize);
      return result;
   }

   inline void deallocate(IndexedFreelist& a, void* ptr)
   {
      CORE_ASSERT_DBGERR(a.memory <= ptr && ptr <= (a.memory + a.objectSize*a.arraySize), "Attempting to free a pointer not belonging to indexed freelist");
      union
      {
         u32* asNode;
         void* address;
         u8* asMemory;
      };
      address = ptr;
      *asNode = a.next;
      a.next = ((u32)(asMemory - a.memory)) / a.objectSize;
      CORE_ASSERT_DBGERR((a.memory + a.next*a.objectSize) == asMemory, "Invalid pointer passed to indexed freelist");
   }
}
