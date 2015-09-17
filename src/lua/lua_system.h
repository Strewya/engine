#pragma once
/********************************************
*  contents:   #description
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "lua/lua_include.h"
#include "utility/types.h"
/******* extra headers *******/
#include "lua/lua_stack.h"
#include "utility/memory.h"
/******* end header inclusion *******/

namespace core
{
   /*
    *    This is the lua allocator object. It is in charge of memory allocation, reallocation and deallocation for the Lua VM.
    *    It is initialized by receiving a memory block within which to work in.
    *    Every time Lua allocates a memory chunk, it calls a function provided to it in the LuaSystem::init() function.
    *    This function will call allocate/reallocate/deallocate on the LuaAllocator object.
    *    The allocations are all in chunks of multiples of ALIGNMENT, which currently, for a 64bit build is 16 bytes.
    *    LuaAllocator uses a freelist implementation. The allocator itself contains the head of the freelist, which contains
    *    the memory address of some free memory and its size. That memory address in turn also contains the next free memory address and its size,
    *    recursively until the rest of the free memory is found, which contains nullptr and 0 to indicate there is no more memory.
    *    
    *    head          | 0x100         | 0x150         | 0x200
    *    size: 16      | size: 48      | size: 100000  | size: 0
    *    memory: 0x100 | memory: 0x150 | memory: 0x200 | memory: 0
    *    
    *    All allocations happen in multiple of 16 chunks. Any requested allocation size is increased to the first next multiple of 16.
    *    Then, the freelist is traversed from the head in search of a chunk big enough for the requested increased size.
    *    When a chunk big enough is found, it is removed from teh freelist by pointing the currentFreelist node to point to the one after the chunk being allocated.
    *    Deallocation is done in a similar fashion, the freelist is traversed to find the first free chunk after the memory being freed.
    *    The freelist nodes around that memory are updated so the currently freed memory is a new node occuring in between them.
    *    An additional step is done which checks the newly created chunk if it can be merged with any other free chunk directly surrounding it.
    *    Reallocation simply uses the allocation and deallocation methods to do it's work. If reallocation is done from a bigger memory size to a smaller one,
    *    the same pointer is returned, and if the difference between the old size and new size is >= 16, then the extra memory is freed.
    *    If reallocation si done from a smaller to a bigger memory size, then a new chunk is allocated to suit the bigger size, a memcpy is performed,
    *    and the old memory is freed.
    *    #todo An improvement can be made to the reallocation method, so reallocs from a smaller to a bigger memory size is first checked if it can be
    *    expanded in place.
    *    
    */
   struct LuaAllocator
   {
      const char* m_tag;
      MemoryBlock m_memory;
      u32 m_allocated;
      u32 m_maxAllocated;
      u8* m_topFreeMemoryAddress;
      struct Freelist
      {
         u32 size;
         union
         {
            u8* address;
            Freelist* nextFreelist;
         };
      } m_freelistHead;

      void init(const char* tag, MemoryBlock memory)
      {
         CORE_ASSERT_DBGWRN(m_allocated == 0, "Switching memory while some is already in use!");
         CORE_ASSERT_DBGERR((u32(memory.address) & 0xf) == 0, "Received memory is not 16 byte aligned");
         m_tag = tag;
         m_memory = memory;
         m_freelistHead.address = memory.address;
         m_freelistHead.size = memory.size;
         m_freelistHead.nextFreelist->address = nullptr;
         m_freelistHead.nextFreelist->size = 0;
      }
   };

   inline std::ostream& operator<<(std::ostream& stream, LuaAllocator& a)
   {
      outputAllocatorData(stream, a.m_tag, a.m_memory.size, a.m_allocated, a.m_maxAllocated);
      stream << "   Memory requirement: " << memoryRequirement(a.m_memory.address, a.m_topFreeMemoryAddress) << logLine;
      return stream;
   }

   struct LuaSystem
   {
   public:
      bool init();
      bool shutdown();
      void collectGarbage();

      LuaStack getStack() const;

   private:
      lua_State* m_L;
      LinearAllocator m_staticMemory;
      LuaAllocator m_luaMemory;

      friend LuaSystem* createScriptSystem(MemoryBlock memory);
   };

   LuaSystem* createScriptSystem(MemoryBlock memory);

   void test_luaStack();
}
