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
#define KiloBytes(n) (Bytes(n)*1024ULL)
#define MegaBytes(n) (KiloBytes(n)*1024ULL)
#define GigaBytes(n) (MegaBytes(n)*1024ULL)
#define TeraBytes(n) (GigaBytes(n)*1024ULL)

namespace core
{
   /************************************************************************/
   /* Utility stream operations                                            */
   /************************************************************************/
   struct byteSizes
   {
      u64 m_B;

      byteSizes(u64 B) : m_B(B) {}
   };
   inline std::ostream& operator<<(std::ostream& stream, byteSizes bs)
   {
      u64 B = bs.m_B;
      f32 kB = B / 1024.0f;
      f32 MB = kB / 1024;
      stream << B << " B / " << std::fixed << kB << " kB / " << MB << " MB" << std::defaultfloat;
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

   inline void outputAllocatorData(std::ostream& stream, const char* tag, u64 totalSize, u64 allocated, u64 peakAllocation)
   {
      stream << "Memory usage for allocator '" << tag << "':" << logLine;
      stream << "   Total memory: " << byteSizes(totalSize) << logLine;
      stream << "   Currently using: " << byteSizes(allocated) << logLine;
      stream << "   Peak usage: " << byteSizes(peakAllocation) << logLine;
   }

   /************************************************************************/
   /* Base memory allocator done on program start                          */
   /************************************************************************/
   struct LargeLinearAllocator
   {
      const char* m_tag;
      u8* m_memory;
      u64 m_size;
      u64 m_allocated;
      u64 m_maxAllocated;

      void init(const char* tag, void* memory, u64 size)
      {
         CORE_ASSERT_DBGERR(m_memory == nullptr, "Attempting to initialize a large allocator that has already been initialized.");
         m_tag = tag;
         m_memory = (u8*)memory;
         m_size = size;
      }
   };

   inline std::ostream& operator<<(std::ostream& stream, LargeLinearAllocator& lla)
   {
      outputAllocatorData(stream, lla.m_tag, lla.m_size, lla.m_allocated, lla.m_maxAllocated);
      return stream;
   }

   struct MemoryBlock
   {
      u8* address;
      u32 size;
   };

   inline MemoryBlock allocateBlock(LargeLinearAllocator& a, u32 size)
   {
      MemoryBlock result{};
      auto* address = a.m_memory + a.m_allocated;
      if( a.m_allocated + size <= a.m_size )
      {
         result.address = address;
         result.size = size;
         a.m_allocated += size;
         a.m_maxAllocated = max(a.m_maxAllocated, a.m_allocated);
      }
      return result;
   }
   inline MemoryBlock allocateBlock(LargeLinearAllocator& a)
   {
      MemoryBlock result{};
      if( a.m_allocated != a.m_size )
      {
         result.size = a.m_size - a.m_allocated;
         result.address = a.m_memory + a.m_allocated;
         a.m_maxAllocated = a.m_allocated = a.m_size;
      }
      return result;
   }


   // Fixed memory manager. Only allocations are supported, frees are not.
   struct LinearAllocator
   {
      const char* m_tag;
      MemoryBlock m_memory;
      u32 m_allocated;
      u32 m_maxAllocated;

      void init(const char* tag, MemoryBlock memory)
      {
         CORE_ASSERT_DBGWRN(m_allocated == 0, "Switching allocator memory that has already allocated memory!");
         m_tag = tag;
         m_memory = memory;
      }
   };
   inline std::ostream& operator<<(std::ostream& stream, LinearAllocator& a)
   {
      outputAllocatorData(stream, a.m_tag, a.m_memory.size, a.m_allocated, a.m_maxAllocated);
      return stream;
   }

   inline void* allocate(LinearAllocator& a, u32 size, u32 align)
   {
      u8* result = a.m_memory.address + a.m_allocated;
      align = align - (u32)result % align;
      size += align;
      if( a.m_allocated + size <= a.m_memory.size )
      {
         result += align;
         a.m_allocated += size;
         a.m_maxAllocated = max(a.m_maxAllocated, a.m_allocated);
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
   inline MemoryBlock allocateBlock(LinearAllocator& a, u32 size)
   {
      MemoryBlock result{};
      result.address = (u8*)allocate(a, size, 1);
      result.size = size;
      return result;
   }
   inline MemoryBlock allocateBlock(LinearAllocator& a)
   {
      MemoryBlock result{};
      if( a.m_allocated != a.m_memory.size )
      {
         result.size = a.m_memory.size - a.m_allocated;
         result.address = a.m_memory.address + a.m_allocated;
         a.m_maxAllocated = a.m_allocated = a.m_memory.size;
      }
      return result;
   }


   struct StackAllocator
   {
      const char* m_tag;
      MemoryBlock m_memory;
      u32 m_allocated;
      u32 m_maxAllocated;

      void init(const char* tag, MemoryBlock memory)
      {
         CORE_ASSERT_DBGWRN(m_allocated == 0, "Switching allocator memory that has some memory allocated!");
         m_tag = tag;
         m_memory = memory;
      }
   };
   inline std::ostream& operator<<(std::ostream& stream, StackAllocator& a)
   {
      outputAllocatorData(stream, a.m_tag, a.m_memory.size, a.m_allocated, a.m_maxAllocated);
      return stream;
   }

   inline void* allocate(StackAllocator& a, u32 size, u32 align)
   {
      u8* result = a.m_memory.address + a.m_allocated;
      if( (u32)result % align != 0 )
      {
         align = align - (u32)result % align;
      }
      size += align;
      if( a.m_allocated + size <= a.m_memory.size )
      {
         result += align;
         a.m_allocated += size;
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
      a.m_allocated = 0;
   }
   inline void deallocate(StackAllocator& a, void* ptr)
   {
      CORE_ASSERT_DBGERR(a.m_memory.address <= ptr && ptr < a.m_memory.address + a.m_memory.size, "Attempting to free a pointer not owned by allocator ", a.m_tag);
      CORE_ASSERT_DBGERR(a.m_memory.address <= ptr && ptr < a.m_memory.address + a.m_allocated, "Attempting to free an already free pointer in ", a.m_tag);

      u32 alignment = *((u8*)ptr - 1);
      u32 size = (u32)(((u8*)a.m_memory.address + a.m_allocated) - (u8*)ptr);
      size += alignment;
      a.m_allocated -= size;
   }
   inline MemoryBlock allocateBlock(StackAllocator& a, u32 size)
   {
      MemoryBlock result{};
      result.address = (u8*)allocate(a, size, 1);
      result.size = size;
      return result;
   }
   inline MemoryBlock allocateBlock(StackAllocator& a)
   {
      MemoryBlock result{};
      if( a.m_allocated != a.m_memory.size )
      {
         result.size = a.m_memory.size - a.m_allocated;
         result.address = a.m_memory.address + a.m_allocated;
         a.m_maxAllocated = a.m_allocated = a.m_memory.size;
      }
      return result;
   }
   void deallocateBlock(StackAllocator& a, MemoryBlock memory)
   {
      CORE_ASSERT_DBGERR(a.m_memory.address <= memory.address && memory.address < a.m_memory.address + a.m_memory.size, "Attempting to free a pointer not owned by allocator ", a.m_tag);
      CORE_ASSERT_DBGERR(a.m_memory.address <= memory.address && memory.address < a.m_memory.address + a.m_allocated, "Attempting to free an already free pointer in ", a.m_tag);

      a.m_allocated -= memory.size;
   }



   struct FixedSizeFreelistAllocator
   {
      struct Node
      {
         Node* next;
      };

      const char* m_tag;
      MemoryBlock m_memory;
      u32 m_allocated;
      u32 m_maxAllocated;
      Node m_head;

      template<typename T>
      void init(const char* tag, MemoryBlock memory, u32 slots)
      {
         CORE_ASSERT_DBGWRN(m_allocated == 0, "Switching freelist memory while some of it is in use!");
         m_tag = tag;
         m_memory = memory;

         static_assert(sizeof(T) >= sizeof(Node), "Cannot use plain freelist for type too small");
         CORE_ASSERT_DBGERR(m_memory.address != nullptr, "Cannot setup freelist ", m_tag, " without it being initialized!");

         T* start = (T*)m_memory.address;
         CORE_ASSERT_DBGERR(m_memory.address + m_memory.size < (start + slots), "Trying to use too much slots for available memory in freelist ", m_tag);

         Node* runner = &m_head;
         for( u32 i = 0; i < slots; ++i )
         {
            runner->next = (Node*)(start + i);
            runner = runner->next;
         }
         runner->next = nullptr;
      }
   };
   inline std::ostream& operator<<(std::ostream& stream, FixedSizeFreelistAllocator& a)
   {
      outputAllocatorData(stream, a.m_tag, a.m_memory.size, a.m_allocated, a.m_maxAllocated);
      return stream;
   }

   inline void* allocate(FixedSizeFreelistAllocator& a)
   {
      if( a.m_head.next == nullptr )
      {
         return nullptr;
      }
      union
      {
         FixedSizeFreelistAllocator::Node* head;
         void* result;
      };
      head = a.m_head.next;
      a.m_head.next = head->next;
      return result;
   }
   inline void deallocate(FixedSizeFreelistAllocator& a, void* ptr)
   {
      union
      {
         FixedSizeFreelistAllocator::Node* head;
         void* address;
      };
      address = ptr;
      head->next = a.m_head.next;
      a.m_head.next = head;
   }



   struct ChunkFreelistAllocator
   {
      struct Node
      {
         u64 size;
         union
         {
            void* address;
            u8* byte;
            Node* next;
         };
      };

      const char* m_tag;
      MemoryBlock m_memory;
      void* m_topFreeMemoryAddress;
      Node m_head;
      u32 m_allocated;
      u32 m_maxAllocated;
      u32 m_alignment;

      void init(const char* tag, MemoryBlock memory, u32 alignment)
      {
         CORE_ASSERT_DBGWRN(m_allocated == 0, "Switching memory while some is already in use!");
         CORE_ASSERT_DBGERR((u32(memory.address) % alignment) == 0, "Received memory is not 16 byte aligned");
         m_tag = tag;
         m_memory = memory;
         m_topFreeMemoryAddress = memory.address;
         m_head.address = memory.address;
         m_head.size = memory.size;
         m_head.next->address = nullptr;
         m_head.next->size = 0;
         m_allocated = m_maxAllocated = 0;
         m_alignment = alignment >= __alignof(Node) ? alignment : __alignof(Node);
      }
   };
   inline std::ostream& operator<<(std::ostream& stream, ChunkFreelistAllocator& a)
   {
      outputAllocatorData(stream, a.m_tag, a.m_memory.size, a.m_allocated, a.m_maxAllocated);
      stream << "   Memory requirement: " << memoryRequirement(a.m_memory.address, a.m_topFreeMemoryAddress) << logLine;
      return stream;
   }

   void* allocate(ChunkFreelistAllocator& a, u32 size)
   {
      u32 sizeExpansionForFreelistPurposes = (a.m_alignment - (size % a.m_alignment));
      sizeExpansionForFreelistPurposes = sizeExpansionForFreelistPurposes & (a.m_alignment - 1);

      size += sizeExpansionForFreelistPurposes;
      CORE_ASSERT_DBGERR((size % a.m_alignment) == 0, "The requested size should always be a multiple of ", a.m_alignment, " after expansion.");
      CORE_ASSERT_DBGERR(size >= sizeof(ChunkFreelistAllocator::Node), "The requested size should be at least ", sizeof(ChunkFreelistAllocator::Node), " bytes after expansion.");

      ChunkFreelistAllocator::Node* currentNode = &a.m_head;
      while( currentNode->address && currentNode->size < size )
      {
         currentNode = currentNode->next;
      }

      if( currentNode->address == nullptr )
      {
         //case where we traversed the entire freelist and found no slot big enough for the requested size
         return nullptr;
      }

      //case where we found a slot big enough for the requested size
      u8* result = currentNode->byte;
      CORE_ASSERT_DBGERR(((u32)result % a.m_alignment) == 0, "The returned address should always be aligned to ", a.m_alignment, " bytes.");

      currentNode->size -= size;
      if( currentNode->size )
      {
         //case where there is more room in the slot after the requested size is removed
         ChunkFreelistAllocator::Node* unmoved = currentNode->next;
         currentNode->byte += size;
         currentNode->next->address = unmoved->address;
         currentNode->next->size = unmoved->size;
         if( currentNode->next->address == nullptr && currentNode->address > a.m_topFreeMemoryAddress )
         {
            a.m_topFreeMemoryAddress = currentNode->address;
         }
      }
      else
      {
         //case where the slot is exhausted by the requested size
         currentNode->size = currentNode->next->size;
         currentNode->address = currentNode->next->address;
      }
      a.m_allocated += size;
      a.m_maxAllocated = max(a.m_maxAllocated, a.m_allocated);
      memset(result, 0, size);

      return result;
   }
}
