//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "lua/lua_system.h"
/******* c++ headers *******/
#include <cstdio>
/******* extra headers *******/
#include "utility/memory.h"
#include "utility/utility.h"
/******* end headers *******/

namespace core
{
   int luaPrint(lua_State* L)
   {
      i32 n = lua_gettop(L);
      for( i32 i = 1; i <= n; ++i )
      {
         switch( lua_type(L, i) )
         {
            case LUA_TNUMBER:
            {
               CORE_LOG_DEBUG(lua_tonumber(L, i));
            } break;
            case LUA_TSTRING:
            {
               CORE_LOG_DEBUG(lua_tostring(L, i));
            } break;
            case LUA_TBOOLEAN:
            {
               CORE_LOG_DEBUG(lua_toboolean(L, i) == 1 ? "true" : "false");
            } break;
            default:
            {
               CORE_LOG_DEBUG(lua_typename(L, i), "(", lua_tonumber(L, i), ")");
            } break;
         }
      }
      return 0;
   }

   int luaErrHandler(lua_State* L)
   {
      lua_pushnil(L);
      return 2;
   }

#define ALIGNMENT 16

   void* allocate(LuaAllocator& a, u32 size)
   {
      u32 sizeExpansionForFreelistPurposes = (ALIGNMENT - (size % ALIGNMENT));
      sizeExpansionForFreelistPurposes = sizeExpansionForFreelistPurposes & 0xF;

      size += sizeExpansionForFreelistPurposes;
      CORE_ASSERT_DBGERR((size % ALIGNMENT) == 0, "The requested size should always be a multiple of 16 after expansion.");
      CORE_ASSERT_DBGERR(size >= 16, "The requested size should be at least 16 bytes after expansion.");

      LuaAllocator::Freelist* currentFreelist = &a.m_freelistHead;
      while( currentFreelist->address && currentFreelist->size < size )
      {
         currentFreelist = currentFreelist->nextFreelist;
      }

      if( currentFreelist->address == nullptr )
      {
         //case where we traversed the entire freelist and found no slot big enough for the requested size
         return nullptr;
      }

      //case where we found a slot big enough for the requested size
      u8* result = currentFreelist->address;
      CORE_ASSERT_DBGERR(((u32)result % ALIGNMENT) == 0, "The returned address should always be aligned to 16 bytes.");

      currentFreelist->size -= size;
      if( currentFreelist->size )
      {
         //case where there is more room in the slot after the requested size is removed
         LuaAllocator::Freelist* unmoved = currentFreelist->nextFreelist;
         currentFreelist->address += size;
         currentFreelist->nextFreelist->address = unmoved->address;
         currentFreelist->nextFreelist->size = unmoved->size;
         if( currentFreelist->nextFreelist->address == nullptr && currentFreelist->address > a.m_topFreeMemoryAddress )
         {
            a.m_topFreeMemoryAddress = currentFreelist->address;
         }
      }
      else
      {
         //case where the slot is exhausted by the requested size
         currentFreelist->size = currentFreelist->nextFreelist->size;
         currentFreelist->address = currentFreelist->nextFreelist->address;
      }
      a.m_allocated += size;
      a.m_maxAllocated = max(a.m_maxAllocated, a.m_allocated);
      memset(result, 0, size);

      return result;
   }

   void deallocate(LuaAllocator& a, void* ptr, u32 size)
   {
      if( !ptr )
      {
         return;
      }
      u32 sizeExpansionForFreelistPurposes = (ALIGNMENT - (size % ALIGNMENT));
      sizeExpansionForFreelistPurposes = sizeExpansionForFreelistPurposes & 0xF;
      size += sizeExpansionForFreelistPurposes;

      CORE_ASSERT_DBGERR((size % ALIGNMENT) == 0, "The size should always be a multiple of 16 after expansion.");
      CORE_ASSERT_DBGERR(size >= 16, "The size should be at least 16 bytes after expansion.");
      CORE_ASSERT_DBGERR(((u32)ptr % ALIGNMENT) == 0, "Every address returned should be aligned to 16 bytes.");

      LuaAllocator::Freelist* previousFreelist = nullptr;
      LuaAllocator::Freelist* currentFreelist = &a.m_freelistHead;
      while( currentFreelist->address && currentFreelist->address < ptr )
      {
         previousFreelist = currentFreelist;
         currentFreelist = currentFreelist->nextFreelist;
      }

      LuaAllocator::Freelist* newFreelist = (LuaAllocator::Freelist*)ptr;
      newFreelist->address = currentFreelist->address;
      newFreelist->size = currentFreelist->size;
      currentFreelist->nextFreelist = newFreelist;
      currentFreelist->size = size;

      while( previousFreelist && previousFreelist->address + previousFreelist->size == currentFreelist->address )
      {
         previousFreelist->size += currentFreelist->size;
         currentFreelist->size = currentFreelist->nextFreelist->size;
         currentFreelist->address = currentFreelist->nextFreelist->address;
      }

      while( currentFreelist->address && currentFreelist->address + currentFreelist->size == newFreelist->address )
      {
         currentFreelist->size += newFreelist->size;
         newFreelist->size = newFreelist->nextFreelist->size;
         newFreelist->address = newFreelist->nextFreelist->address;
      }

      a.m_allocated -= size;
   }

   void* reallocate(LuaAllocator& a, void* ptr, u32 osize, u32 nsize)
   {
      void* result = nullptr;
      if( nsize < osize )
      {
         //if the new size is less than the old size, we can just return the original pointer, and release the extra size
         //first we need to modify the size to be multiple of 16
         u32 sizeExpansionForFreelistPurposes = (ALIGNMENT - (nsize % ALIGNMENT));
         sizeExpansionForFreelistPurposes = sizeExpansionForFreelistPurposes & 0xF;
         nsize += sizeExpansionForFreelistPurposes;

         void* ptrToDeallocate = (u8*)ptr + nsize;
         if( osize - nsize )
         {
            deallocate(a, ptrToDeallocate, osize - nsize);
         }
         result = ptr;
      }
      else //nsize > osize
      {
         // #todo try to allocate memory right after the current pointer to just expand the pointer in place, will save the memcpy
         void* newPtr = allocate(a, nsize);
         if( newPtr )
         {
            memcpy(newPtr, ptr, osize);
            deallocate(a, ptr, osize);
         }
         result = newPtr;
      }

      return result;
   }


   void* lua_allocateCustom(void* ud, void* ptr, size_t osize, size_t nsize)
   {
      LuaAllocator* a = (LuaAllocator*)ud;
      void* result = nullptr;
      const char* action = "Unknown";
      if( nsize == 0 )
      {
         action = "Deallocating";
         deallocate(*a, ptr, (u32)osize);
      }
      else if( osize == 0 && nsize > 0 )
      {
         action = "Allocating";
         result = allocate(*a, (u32)nsize);
      }
      else if( osize > 0 && nsize > 0 )
      {
         action = "Reallocating";
         if( osize != nsize )
         {
            result = reallocate(*a, ptr, (u32)osize, (u32)nsize);
         }
         else
         {
            result = ptr;
         }
      }

      CORE_LOG_DEBUG(action, " ptr/osize/nsize: ", memoryAddress(ptr), "/", osize, "/", nsize);
      return result;
   }

   void* lua_allocate(void* ud, void* ptr, size_t osize, size_t nsize)
   {
      CORE_LOG_DEBUG("Lua allocation, ptr/osize/nsize: ", (u64)ptr, "/", osize, "/", nsize);

      (void)ud;  (void)osize;  /* not used */
      if( nsize == 0 )
      {
         free(ptr);
         return nullptr;
      }

      auto* result = realloc(ptr, nsize);
      CORE_LOG_DEBUG("Realloc memory address and alignment: ", (u32)result, " /// ", (u32)result & 0xf);
      return result;
   }


   LuaSystem* createScriptSystem(MemoryBlock memory)
   {
      LinearAllocator a;
      a.init("Lua allocator", memory);
      LuaSystem* result = allocate<LuaSystem>(a);
      result->m_staticMemory = a;
      return result;
   }

   static void test_luaAllocator(MemoryBlock memory);

   bool LuaSystem::init()
   {
      CORE_INIT_START(LuaSystem);

      //test_luaAllocator(memory);

      m_luaMemory.init("Lua system", allocateBlock(m_staticMemory));

      m_L = lua_newstate(lua_allocateCustom, &m_luaMemory);

      CORE_STATUS_AND(m_L != nullptr);
      if( CORE_STATUS_OK )
      {
#ifdef DEPLOY
         lua_pushcfunction(m_L, luaopen_base);
         lua_pushliteral(m_L, "");
         lua_call(m_L, 1, 0);

         lua_pushcfunction(m_L, luaopen_table);
         lua_pushliteral(m_L, LUA_TABLIBNAME);
         lua_call(m_L, 1, 0);

         lua_pushcfunction(m_L, luaopen_package);
         lua_pushliteral(m_L, LUA_LOADLIBNAME);
         lua_call(m_L, 1, 0);

         lua_pushcfunction(m_L, luaopen_string);
         lua_pushliteral(m_L, LUA_STRLIBNAME);
         lua_call(m_L, 1, 0);

         lua_pushcfunction(m_L, luaopen_math);
         lua_pushliteral(m_L, LUA_MATHLIBNAME);
         lua_call(m_L, 1, 0);
#else
         luaL_openlibs(m_L);
#endif

         //tolua_core_open(m_L);
         lua_register(m_L, "print", luaPrint);

         const char* depend = R"rawLuaCode(
Lua = {};
global_dependency_table = {};
global_dependency_missing = nil;

function depend(filename)
   if(not global_dependency_table[filename]) then
      global_dependency_missing = filename;
      error('Missing dependency');
   end;
end;

function class(name)
   Lua[name] = {};
   Lua[name].__index = Lua[name];
   setmetatable(Lua[name], { __call = function(cls, ...) return cls.new(...); end; });
   Lua[name].new = function()
      return {};
   end;
end
)rawLuaCode";

         CORE_STATUS_AND(luaL_dostring(m_L, depend) == 0);
      }

      CORE_INIT_END;
   }

   bool LuaSystem::shutdown()
   {
      CORE_SHUTDOWN_START(LuaSystem);
      lua_close(m_L);
      CORE_LOG_DEBUG(m_staticMemory, m_luaMemory);
      CORE_SHUTDOWN_END;
   }

   void LuaSystem::collectGarbage()
   {
      lua_gc(m_L, LUA_GCSTEP, 20);
   }

   LuaStack LuaSystem::getStack() const
   {
      return LuaStack(m_L);
   }



   void test_luaAllocator(MemoryBlock memory)
   {
      LuaAllocator la{};
      la.init("Lua allocator test", memory);

      u8* startAddress = la.m_memory.address;

      void* a30 = allocate(la, 30);
      CORE_ASSERT_DBGERR(a30 == startAddress, "Playground error");
      CORE_ASSERT_DBGERR(la.m_allocated == 30, "Playground error");
      CORE_ASSERT_DBGERR(la.m_freelistHead.address == startAddress + 30, "Playground error");
      CORE_ASSERT_DBGERR(la.m_freelistHead.size == la.m_memory.size - 30, "Playground error");
      CORE_ASSERT_DBGERR(la.m_freelistHead.nextFreelist->address == nullptr, "Playground error");
      CORE_ASSERT_DBGERR(la.m_freelistHead.nextFreelist->size == 0, "Playground error");

      void* b40 = allocate(la, 40);
      CORE_ASSERT_DBGERR(b40 == startAddress + 30, "Playground error");
      CORE_ASSERT_DBGERR(la.m_allocated == 70, "Playground error");
      CORE_ASSERT_DBGERR(la.m_freelistHead.address == startAddress + 30 + 40, "Playground error");
      CORE_ASSERT_DBGERR(la.m_freelistHead.size == la.m_memory.size - 30 - 40, "Playground error");
      CORE_ASSERT_DBGERR(la.m_freelistHead.nextFreelist->address == nullptr, "Playground error");
      CORE_ASSERT_DBGERR(la.m_freelistHead.nextFreelist->size == 0, "Playground error");

      void* c50 = allocate(la, 50);
      CORE_ASSERT_DBGERR(c50 == startAddress + 30 + 40, "Playground error");
      CORE_ASSERT_DBGERR(la.m_allocated == 120, "Playground error");
      CORE_ASSERT_DBGERR(la.m_freelistHead.address == startAddress + 30 + 40 + 50, "Playground error");
      CORE_ASSERT_DBGERR(la.m_freelistHead.size == la.m_memory.size - 30 - 40 - 50, "Playground error");
      CORE_ASSERT_DBGERR(la.m_freelistHead.nextFreelist->address == nullptr, "Playground error");
      CORE_ASSERT_DBGERR(la.m_freelistHead.nextFreelist->size == 0, "Playground error");

      deallocate(la, b40, 40);
      CORE_ASSERT_DBGERR(la.m_allocated == 80, "Playground error");
      CORE_ASSERT_DBGERR(la.m_freelistHead.address == startAddress + 30, "Playground error");
      CORE_ASSERT_DBGERR(la.m_freelistHead.size == 40, "Playground error");
      CORE_ASSERT_DBGERR(la.m_freelistHead.nextFreelist->address == startAddress + 30 + 40 + 50, "Playground error");
      CORE_ASSERT_DBGERR(la.m_freelistHead.nextFreelist->size == la.m_memory.size - 30 - 40 - 50, "Playground error");
      CORE_ASSERT_DBGERR(la.m_freelistHead.nextFreelist->nextFreelist->address == nullptr, "Playground error");
      CORE_ASSERT_DBGERR(la.m_freelistHead.nextFreelist->nextFreelist->size == 0, "Playground error");

      void* d20 = allocate(la, 20);
      CORE_ASSERT_DBGERR(d20 == startAddress + 30, "Playground error");
      CORE_ASSERT_DBGERR(la.m_allocated == 100, "Playground error");
      CORE_ASSERT_DBGERR(la.m_freelistHead.address == startAddress + 30 + 20, "Playground error");
      CORE_ASSERT_DBGERR(la.m_freelistHead.size == 20, "Playground error");
      CORE_ASSERT_DBGERR(la.m_freelistHead.nextFreelist->address == startAddress + 30 + 40 + 50, "Playground error");
      CORE_ASSERT_DBGERR(la.m_freelistHead.nextFreelist->size == la.m_memory.size - 30 - 40 - 50, "Playground error");
      CORE_ASSERT_DBGERR(la.m_freelistHead.nextFreelist->nextFreelist->address == nullptr, "Playground error");
      CORE_ASSERT_DBGERR(la.m_freelistHead.nextFreelist->nextFreelist->size == 0, "Playground error");

      void* e20 = allocate(la, 20);
      CORE_ASSERT_DBGERR(e20 == startAddress + 30 + 20, "Playground error");
      CORE_ASSERT_DBGERR(la.m_allocated == 120, "Playground error");
      CORE_ASSERT_DBGERR(la.m_freelistHead.address == startAddress + 30 + 40 + 50, "Playground error");
      CORE_ASSERT_DBGERR(la.m_freelistHead.size == la.m_memory.size - 30 - 40 - 50, "Playground error");
      CORE_ASSERT_DBGERR(la.m_freelistHead.nextFreelist->address == nullptr, "Playground error");
      CORE_ASSERT_DBGERR(la.m_freelistHead.nextFreelist->size == 0, "Playground error");

      deallocate(la, e20, 20);
      CORE_ASSERT_DBGERR(la.m_allocated == 100, "Playground error");
      deallocate(la, d20, 20);
      CORE_ASSERT_DBGERR(la.m_allocated == 80, "Playground error");

      void* f60 = allocate(la, 60);
      CORE_ASSERT_DBGERR(f60 == startAddress + 30 + 40 + 50, "Playground error");
      CORE_ASSERT_DBGERR(la.m_allocated == 140, "Playground error");

      deallocate(la, c50, 50);
      CORE_ASSERT_DBGERR(la.m_allocated == 90, "Playground error");
      deallocate(la, f60, 60);
      CORE_ASSERT_DBGERR(la.m_allocated == 30, "Playground error");
      deallocate(la, a30, 30);
      CORE_ASSERT_DBGERR(la.m_allocated == 0, "Playground error");
   }
}
