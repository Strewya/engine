//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "lua/lua_system.h"
/******* c++ headers *******/
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

   void* allocate(LuaAllocator& a, u32 requestedSize)
   {
      LuaAllocator::Freelist* currentFreelist = &a.freelistHead;
      while( currentFreelist->address && currentFreelist->size < requestedSize )
      {
         currentFreelist = currentFreelist->nextFreelist;
      }

      if( currentFreelist->address == nullptr )
      {
         //case where we traversed the entire freelist and found no slot big enough for the requested size
         return nullptr;
      }

      //case where we found a slot big enough for the requested size
      void* result = currentFreelist->address;

      currentFreelist->size -= requestedSize;
      if( currentFreelist->size )
      {
         //case where there is more room in the slot after the requested size is removed
         LuaAllocator::Freelist* unmoved = currentFreelist->nextFreelist;
         currentFreelist->address += requestedSize;
         currentFreelist->nextFreelist->address = unmoved->address;
         currentFreelist->nextFreelist->size = unmoved->size;
      }
      else
      {
         //case where the slot is exhausted by the requested size
         currentFreelist->size = currentFreelist->nextFreelist->size;
         currentFreelist->address = currentFreelist->nextFreelist->address;
      }
      a.allocated += requestedSize;
      a.maxAllocated = max(a.maxAllocated, a.allocated);
      memset(result, 0, requestedSize);
      return result;
   }

   void deallocate(LuaAllocator& a, void* ptr, u32 size)
   {
      if( !ptr )
      {
         return;
      }
      LuaAllocator::Freelist* previousFreelist = nullptr;
      LuaAllocator::Freelist* currentFreelist = &a.freelistHead;
      while( currentFreelist->address && currentFreelist->address < ptr )
      {
         previousFreelist = currentFreelist;
         currentFreelist = currentFreelist->nextFreelist;
      }

      LuaAllocator::Freelist* newFreelistNode = (LuaAllocator::Freelist*)ptr;
      newFreelistNode->address = currentFreelist->address;
      newFreelistNode->size = currentFreelist->size;
      currentFreelist->nextFreelist = newFreelistNode;
      currentFreelist->size = size;

      while( previousFreelist && previousFreelist->address + previousFreelist->size == currentFreelist->address )
      {
         previousFreelist->size += currentFreelist->size;
         currentFreelist->size = currentFreelist->nextFreelist->size;
         currentFreelist->address = currentFreelist->nextFreelist->address;
      }

      a.allocated -= size;
   }

   void* reallocate(LuaAllocator& a, void* ptr, u32 osize, u32 nsize)
   {
      void* result = nullptr;
      if( nsize < osize )
      {
         //if the new size is less than the old size, we can just return the original pointer, and release the extra size
         void* ptrToDeallocate = (u8*)ptr + nsize;
         deallocate(a, ptrToDeallocate, osize - nsize);
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
      }

      return result;
   }


   void* lua_allocateCustom(void* ud, void* ptr, size_t osize, size_t nsize)
   {
      osize = osize ? max(osize, sizeof(LuaAllocator::Freelist)) : osize;
      nsize = nsize ? max(nsize, sizeof(LuaAllocator::Freelist)) : nsize;
      LuaAllocator* a = (LuaAllocator*)ud;
      void* result = nullptr;
      if( nsize == 0 )
      {
         CORE_LOG_DEBUG("Deallocating ptr/osize/nsize: ", (u64)ptr, "/", osize, "/", nsize);
         deallocate(*a, ptr, (u32)osize);
      }
      else if( osize == 0 && nsize > 0 )
      {
         CORE_LOG_DEBUG("Allocating ptr/osize/nsize: ", (u64)ptr, "/", osize, "/", nsize);
         result = allocate(*a, (u32)nsize);
      }
      else if( osize > 0 && nsize > 0 && osize != nsize )
      {
         CORE_LOG_DEBUG("Reallocating ptr/osize/nsize: ", (u64)ptr, "/", osize, "/", nsize);
         result = reallocate(*a, ptr, (u32)osize, (u32)nsize);
      }
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, result != (void*)0x000004000000FFC6);
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
      return result;
   }

   static void test_luaAllocator(LinearAllocator a);

   bool LuaSystem::init(LinearAllocator& a, u32 memorySize)
   {
      CORE_INIT_START(LuaSystem);

      //test_luaAllocator(a);

      m_allocator.tag = "Lua system";
      m_allocator.init(allocate(a, memorySize, 1), memorySize);

      m_L = lua_newstate(lua_allocateCustom, &m_allocator);

      CORE_STATUS_AND(m_L != nullptr);
      if( CORE_STATUS_OK )
      {
         u32 t = lua_gettop(m_L);
         lua_pushcfunction(m_L, luaopen_base);
         lua_pushliteral(m_L, "");
         lua_call(m_L, 1, 0);
         t = lua_gettop(m_L);
         lua_pushcfunction(m_L, luaopen_table);
         lua_pushliteral(m_L, LUA_TABLIBNAME);
         lua_call(m_L, 1, 0);
         t = lua_gettop(m_L);
         lua_pushcfunction(m_L, luaopen_os);
         lua_pushliteral(m_L, LUA_OSLIBNAME);
         lua_call(m_L, 1, 0);
         t = lua_gettop(m_L);
         lua_pushcfunction(m_L, luaopen_debug);
         lua_pushliteral(m_L, LUA_DBLIBNAME);
         lua_call(m_L, 1, 0);
         t = lua_gettop(m_L);
         lua_pushcfunction(m_L, luaopen_package);
         lua_pushliteral(m_L, LUA_LOADLIBNAME);
         lua_call(m_L, 1, 0);
         t = lua_gettop(m_L);
         lua_pushcfunction(m_L, luaopen_io);
         lua_pushliteral(m_L, LUA_IOLIBNAME);
         lua_call(m_L, 1, 0);
         t = lua_gettop(m_L);
         lua_pushcfunction(m_L, luaopen_string);
         lua_pushliteral(m_L, LUA_STRLIBNAME);
         lua_call(m_L, 1, 0);
         t = lua_gettop(m_L);
         lua_pushcfunction(m_L, luaopen_math);
         lua_pushliteral(m_L, LUA_MATHLIBNAME);
         lua_call(m_L, 1, 0);
         t = lua_gettop(m_L);
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
      CORE_LOG_DEBUG(m_allocator);
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



   void test_luaAllocator(LinearAllocator a)
   {
      LuaAllocator la{};

      u32 size = Megabytes(30);
      la.init(allocate(a, size, 1), size);
      u8* startAddress = la.memory;

      void* a30 = allocate(la, 30);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, a30 == startAddress);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.allocated == 30);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.freelistHead.address == startAddress + 30);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.freelistHead.size == size - 30);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.freelistHead.nextFreelist->address == nullptr);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.freelistHead.nextFreelist->size == 0);

      void* b40 = allocate(la, 40);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, b40 == startAddress + 30);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.allocated == 70);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.freelistHead.address == startAddress + 30 + 40);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.freelistHead.size == size - 30 - 40);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.freelistHead.nextFreelist->address == nullptr);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.freelistHead.nextFreelist->size == 0);

      void* c50 = allocate(la, 50);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, c50 == startAddress + 30 + 40);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.allocated == 120);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.freelistHead.address == startAddress + 30 + 40 + 50);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.freelistHead.size == size - 30 - 40 - 50);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.freelistHead.nextFreelist->address == nullptr);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.freelistHead.nextFreelist->size == 0);

      deallocate(la, b40, 40);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.allocated == 80);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.freelistHead.address == startAddress + 30);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.freelistHead.size == 40);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.freelistHead.nextFreelist->address == startAddress + 30 + 40 + 50);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.freelistHead.nextFreelist->size == size - 30 - 40 - 50);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.freelistHead.nextFreelist->nextFreelist->address == nullptr);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.freelistHead.nextFreelist->nextFreelist->size == 0);

      void* d20 = allocate(la, 20);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, d20 == startAddress + 30);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.allocated == 100);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.freelistHead.address == startAddress + 30 + 20);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.freelistHead.size == 20);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.freelistHead.nextFreelist->address == startAddress + 30 + 40 + 50);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.freelistHead.nextFreelist->size == size - 30 - 40 - 50);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.freelistHead.nextFreelist->nextFreelist->address == nullptr);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.freelistHead.nextFreelist->nextFreelist->size == 0);

      void* e20 = allocate(la, 20);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, e20 == startAddress + 30 + 20);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.allocated == 120);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.freelistHead.address == startAddress + 30 + 40 + 50);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.freelistHead.size == size - 30 - 40 - 50);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.freelistHead.nextFreelist->address == nullptr);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.freelistHead.nextFreelist->size == 0);

      deallocate(la, e20, 20);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.allocated == 100);
      deallocate(la, d20, 20);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.allocated == 80);

      void* f60 = allocate(la, 60);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, f60 == startAddress + 30 + 40 + 50);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.allocated == 140);

      deallocate(la, c50, 50);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.allocated == 90);
      deallocate(la, f60, 60);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.allocated == 30);
      deallocate(la, a30, 30);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, la.allocated == 0);
   }
}
