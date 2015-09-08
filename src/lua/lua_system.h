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
   struct LuaAllocator
   {
      const char* tag;
      u8* memory;
      u32 size;
      u32 allocated;
      u32 maxAllocated;
      struct Freelist
      {
         u32 size;
         union
         {
            u8* address;
            Freelist* nextFreelist;
         };
      } freelistHead;

      void init(u8* start, u32 sz)
      {
         allocated = 0;
         memory = start;
         size = sz;
         freelistHead.address = start;
         freelistHead.size = sz;
         freelistHead.nextFreelist->address = nullptr;
         freelistHead.nextFreelist->size = 0;
      }
   };

   inline std::ostream& operator<<(std::ostream& stream, LuaAllocator& a)
   {
      u32 B = a.allocated;
      f32 kB = B / 1024.0f;
      f32 MB = kB / 1024;
      stream << "Memory usage for allocator '" << a.tag << "':" << logLine;
      stream << "   Total memory: " << a.size << logLine;
      stream << "   Max used: " << B << " kB / " << kB << " kB / " << MB << " MB";
      return stream;
   }

   struct LuaSystem
   {
   public:
      bool init(LinearAllocator& a, u32 memorySize);
      bool shutdown();
      void collectGarbage();

      LuaStack getStack() const;

   private:
      lua_State* m_L;
      LuaAllocator m_allocator;
   };

   void test_luaStack();
}
