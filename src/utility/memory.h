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
   std::ostream& operator<<(std::ostream& stream, byteSizes bs);

   struct memoryRequirement
   {
      void* m_start;
      void* m_end;

      memoryRequirement(void* start, void* end) : m_start{start}, m_end{end} {}
   };
   std::ostream& operator<<(std::ostream& stream, memoryRequirement mr);

   struct memoryAddress
   {
      void* m_address;

      memoryAddress(void* address) : m_address(address) {}
   };
   std::ostream& operator<<(std::ostream& stream, memoryAddress ma);

   /************************************************************************
    *              Memory
    *    The main object through which memory allocation and reservation
    *    is done.
    ************************************************************************/
   struct Memory
   {
      void* address;
      u64 remainingBytes;
   };
   inline bool operator==(Memory m, nullptr_t p)
   {
      auto result = m.address == nullptr;
      return result;
   }
   inline bool operator!=(Memory m, nullptr_t p)
   {
      auto result = m.address != nullptr;
      return result;
   }
   inline std::ostream& operator<<(std::ostream& stream, Memory m)
   {
      stream << "Remaining memory: " << m.remainingBytes << logLine;
      return stream;
   }

   inline u32 getAlignmentBytes(Memory m, u32 alignment)
   {
      CORE_ASSERT_DBGERR((alignment != 0) && !(alignment & (alignment - 1)), "Alignment is not power of two!");
      u32 memAlign = reinterpret_cast<u32>(m.address) % alignment;
      if( memAlign )
      {
         alignment -= memAlign;
      }
      return memAlign;
   }

   inline Memory advanceMemory(Memory m, u32 size)
   {
      m.address = static_cast<u8*>(m.address) + size;
      m.remainingBytes -= size;
      return m;
   }

   inline Memory alignMemory(Memory m, u32 alignment)
   {
      alignment = getAlignmentBytes(m, alignment);
      CORE_ASSERT_DBGWRN(alignment == 0, "Losing ", alignment, " bytes due to alignment!");
      auto result = advanceMemory(m, alignment);
      return result;
   }

   inline bool hasMemory(Memory m, u32 size, u32 alignment)
   {
      alignment = getAlignmentBytes(m, alignment);
      auto result = m.remainingBytes >= (size + alignment);
      return result;
   }

   inline Memory allocateMemoryChunk(Memory& m, u32 size, u32 align)
   {
      Memory result{};
      if( hasMemory(m, size, align) )
      {
         result = alignMemory(m, align);
         m = advanceMemory(result, size);
         result.remainingBytes = size;
      }
      return result;
   }

   template<typename T>
   inline T* emplace(Memory& m)
   {
      u32 size = sizeof(T);
      u32 align = __alignof(T);
      if( !hasMemory(m, size, align) )
      {
         return nullptr;
      }
      auto alignedMemory = alignMemory(m, align);
      m = advanceMemory(alignedMemory, size);
      T* result = new(alignedMemory.address)T();
      return result;
   }

   template<typename T>
   inline T* emplaceArray(Memory& m, u32 count)
   {
      u32 size = sizeof(T)*count;
      u32 align = __alignof(T);
      if( !hasMemory(m, size, align) )
      {
         return nullptr;
      }
      auto alignedMemory = alignMemory(m, align);
      m = advanceMemory(alignedMemory, size);
      T* result = static_cast<T*>(alignedMemory.address);
      return result;
   }

#ifdef ALLOCATOR_V1
   /************************************************************************
    *              Interface for all allocators
    *    I have decided that i want to keep the coding sane, and take
    *    the small performance penalty with memory allocation in order
    *    to leverage the use of manual memory allocation, and to have
    *    most functions that need dynamic allocation receive this
    *    interface, so function signatures remain as simple as possible
    ************************************************************************/
   struct Allocator
   {
      /*
       *    virtual functions for concrete allocator overriding
       */
      virtual void* allocateRaw(u32 size, u32 align) = 0;
      virtual void deallocateRaw(void* ptr, u32 size) = 0;
      virtual void outputToStream(std::ostream& stream) = 0;

      /*
       *    templated functions used to allocate types and arrays in an easy manner
       */
      template<typename T> T* allocate()
      {
         void* raw = allocateRaw(sizeof(T), __alignof(T));
         memset(raw, 0, sizeof(T));
         T* result = new(raw)T();
         return result;
      }
      template<typename T> T* allocateArray(u32 N)
      {
         union
         {
            void* asVoid;
            u8* asByte;
            u32* asInt;
         };
         u32 align = max((u32)__alignof(u32), (u32)__alignof(T));
         u32 extraSizeForCount = max((u32)sizeof(u32), align);

         u32 bytes = sizeof(T)*N;
         asVoid = (u8*)allocateRaw(bytes + extraSizeForCount, align);
         memset(asVoid, 0, bytes + extraSizeForCount);
         *asInt = N;

         T* result = (T*)(asByte + extraSizeForCount);
         /*
         for( u32 i = 0U; i < N; ++i )
         {
         new(result + i)T();
         }*/
         return (result);
      }

      /*
       *    templated functions used to deallocate types in an easy manner
       */
      template<typename T> void deallocate(T* ptr)
      {
         ptr->~T();
         deallocateRaw(ptr, sizeof(T));
      }
      template<typename T> void deallocateArray(T* ptr)
      {
         union
         {
            T* array;
            u32* count;
            u8* byte;
         };
         u32 align = max((u32)__alignof(u32), (u32)__alignof(T));
         u32 extraSizeForCount = max((u32)sizeof(u32), align);

         array = ptr;
         byte -= extraSizeForCount;
         auto N = *count;
         /*
         for( u32 i = 0U; i < N; ++i )
         {
         (ptr + i)->~T();
         }
         }*/
         u32 bytes = sizeof(T)*N;
         deallocateRaw(array, bytes + extraSizeForCount);
      }
   };
   inline std::ostream& operator<<(std::ostream& stream, Allocator& a)
   {
      a.outputToStream(stream);
      return stream;
   }
   inline std::ostream& operator<<(std::ostream& stream, Allocator* a)
   {
      a->outputToStream(stream);
      return stream;
   }

   /************************************************************************
    *              Allocators to implement:
    *    HeapAllocator - An allocator that allocates varied sized blocks
    *    and keeps an in-place linked list of free blocks. Look at
    *    dlmalloc, it is pretty much the standard allocator.
    *
    *    FrameAllocator - Also called "arena allocator" or "pointer bump
    *    allocator". An allocator that doesn't deallocate individual blocks
    *    but releases all its memory in one go. That means it has a super
    *    simple internal state (just a pointer to the next free byte and
    *    the remaining bytes of free memory).
    *
    *    PageAllocator - The virtual memory allocator provided by the
    *    system. Allocates memory as a number of "pages". The page size is
    *    OS dependent (typically something like 1K -- 64K). You don't want
    *    to use it for small allocations, but it can be used as a "backing
    *    allocator" for any of the other allocators. Google "virtual memory"
    *    and you should find lots of information.
    *
    *    PoolAllocator - An allocator that has pools for allocations of
    *    certain sizes. For example, one pool may only allocate 16 byte
    *    blocks. The pool can be packed tight in memory, you don't need
    *    headers and footers for the blocks, since they are always the same
    *    size. This saves memory and reduces fragmentation and allocation
    *    time.
    ************************************************************************/

#define TEST_MEMORY_ALLOCATION_PERFORMANCE_OFF
#ifndef TEST_MEMORY_ALLOCATION_PERFORMANCE
   /************************************************************************
    *              LargeAllocator, a type of FrameAllocator
    *    Created on game startup. Initialized with raw memory.
    *    Only allocator that can hold more than 4GB of memory.
    *    Passed to the game by reference. Parent for all other allocators.
    *    Works the same as a FrameAllocator, but for bigger memory sizes.
    ************************************************************************/
   struct MainAllocator : public Allocator
   {
      union
      {
         void* m_voidAddress;
         u8* m_byteAddress;
      };
      u64 m_sizeBytes;
      u64 m_allocatedBytes;
      u32 m_allocations;

      MainAllocator();

      void initializeMemory(void* memory, u64 size);
      void shutdown();

      void* allocateRaw(u32 size, u32 align) override;
      void deallocateRaw(void* ptr, u32 size) override;
      void outputToStream(std::ostream& stream) override;
   };

   /************************************************************************
    *              FrameAllocator
    *    Allocations work by pointer bumping. Deallocations are done only
    *    on the last allocated memory (cannot deallocate memory in the
    *    middle of the allocators memory).
    ************************************************************************/
   struct FrameAllocator : public Allocator
   {
      union
      {
         void* m_voidAddress;
         u8* m_byteAddress;
      };
      u32 m_sizeBytes;
      u32 m_allocatedBytes;
      u32 m_allocations;

      FrameAllocator();

      void acquireMemory(Allocator& parent, u32 sizeBytes);
      void returnMemory(Allocator& parent);

      void* allocateRaw(u32 size, u32 align) override;
      void deallocateRaw(void* ptr, u32 size) override;
      void outputToStream(std::ostream& stream) override;
   };

   /************************************************************************
    *              PoolAllocator
    *    Allocates memory in single chunks of memory of predetermined size
    *    and alignment. Deallocations are supported by returning the chunk
    *    to the list of available slots.
    ************************************************************************/
   struct PoolAllocator : public Allocator
   {
      struct Node
      {
         Node* next;
      };
      void* m_voidAddress;
      Node m_head;
      u32 m_sizeBytes;
      u32 m_allocatedBytes;
      u32 m_allocations;
      u32 m_slotSize;

      PoolAllocator();

      void acquireMemory(Allocator& parent, u32 slotCount, u32 slotSize);
      void returnMemory(Allocator& parent);

      void* allocateRaw(u32 size, u32 align) override;
      void deallocateRaw(void* ptr, u32 size) override;
      void outputToStream(std::ostream& stream) override;
   };

   /*
    *    This is the heap allocator object. It can do memory allocation, deallocation and reallocation.
    *    It is initialized by receiving a memory block within which to work in.
    *    The allocations are all in chunks of multiples of m_alignment, which can be set on initialization.
    *    HeapAllocator uses a freelist implementation. The allocator itself contains the head of the freelist, which contains
    *    the memory address of the next free memory block and its size. That memory address in turn also contains the next free memory address and its size,
    *    recursively until the rest of the free memory is found, which contains nullptr and 0 to indicate there is no more memory.
    *
    *    head          | 0x100         | 0x150         | 0x200
    *    size: 16      | size: 48      | size: 100000  | size: 0
    *    memory: 0x100 | memory: 0x150 | memory: 0x200 | memory: 0
    *
    *    All allocations happen in multiple of m_alignment chunks. Any requested allocation size is increased to the first next multiple of 16.
    *    Then, the freelist is traversed from the head in search of a chunk big enough for the requested increased size.
    *    When a chunk big enough is found, it is removed from the freelist by pointing the currentNode to point to the one after the chunk being allocated.
    *    Deallocation is done in a similar fashion, the freelist is traversed to find the first free chunk after the memory being freed.
    *    The freelist nodes around that memory are updated so the currently freed memory is a new node occuring in between them.
    *    An additional step is done which checks the newly created chunk if it can be merged with any other free chunk directly surrounding it.
    *    Reallocation simply uses the allocation and deallocation methods to do it's work. If reallocation is done from a bigger memory size to a smaller one,
    *    the same pointer is returned, and if the difference between the old size and new size is >= 16, then the extra memory is freed.
    *    If reallocation is done from a smaller to a bigger memory size, then a new chunk is allocated to suit the bigger size, a memcpy is performed,
    *    and the old memory is freed.
    *    An improvement can be made to the reallocation method, so reallocs from a smaller to a bigger memory size is first checked if it can be
    *    expanded in place.
    *    #test The previous todo is implemented, just need to test it works now.
    *
    */
   /************************************************************************
   *              HeapAllocator
   *    Allocates memory in chunks of multiples of 16 bytes aligned to 16
   *    bytes. Deallocates memory by returning all chunks (aka you can ask
   *    for 24 bytes, and you will get 32 bytes).
   ************************************************************************/
   struct HeapAllocator : public Allocator
   {
      struct Node
      {
         u32 size;
         union
         {
            void* voidAddress;
            u8* byteAddress;
            Node* next;
         };
      };
      void* m_voidAddress;
      Node m_head;
      u32 m_sizeBytes;
      u32 m_allocatedBytes;
      u32 m_allocations;

      HeapAllocator();

      void acquireMemory(Allocator& parent, u32 sizeBytes);
      void returnMemory(Allocator& parent);

      void* allocateRaw(u32 size, u32 align) override;
      void deallocateRaw(void* ptr, u32 size) override;
      void outputToStream(std::ostream& stream) override;
   };

#else //!TEST_MEMORY_ALLOCATION_PERFORMANCE

   /************************************************************************
    *              MallocAllocator
    *    Allocator based on malloc, just to see the difference of speed.
    ************************************************************************/
   struct MallocAllocator : public core::Allocator
   {
      void initializeMemory(void* ptr, u64 size)
      {}
      void acquireMemory(Allocator& a, u32 size)
      {}
      void returnMemory(Allocator& a)
      {}

      void* allocateRaw(u32 size, u32 align) override
      {
         void* result = malloc(size);
         return result;
      }

      void deallocateRaw(void* ptr, u32 size) override
      {
         free(ptr);
      }

      void outputToStream(std::ostream& stream) override
      {}
   };

   typedef MallocAllocator MainAllocator;
   typedef MallocAllocator FrameAllocator;
   typedef MallocAllocator PoolAllocator;
   typedef MallocAllocator HeapAllocator;

#endif //TEST_MEMORY_ALLOCATION_PERFORMANCE

#endif //ALLOCATOR_V1
}
