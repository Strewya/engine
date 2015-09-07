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



   struct LuaAllocator
   {
      u8* memory;
      u32 size;
      struct Freelist
      {
         u32 freeSize;
         union
         {
            u8* nextFreeAddress;
            Freelist* nextFreelist;
         };
      } freelistHead;

      void init(u8* start, u32 sz)
      {
         memory = start;
         size = sz;
         freelistHead.nextFreeAddress = start;
         freelistHead.freeSize = sz;
      }
   };

   void* allocate(LuaAllocator& a, u32 size)
   {
      LuaAllocator::Freelist* runner = &a.freelistHead;
      while( runner->freeSize < size )
      {
         runner = runner->nextFreelist;
      }
      void* result = runner->nextFreeAddress;
      runner->nextFreeAddress += size;
      runner->freeSize -= size;
      return result;
   }

   void* reallocate(LuaAllocator& a, void* ptr, u32 osize, u32 nsize)
   {
      return nullptr;
   }

   void deallocate(LuaAllocator& a, void* ptr, u32 size)
   {
      LuaAllocator::Freelist* runner = &a.freelistHead;
      while( runner->nextFreeAddress > ptr )
      {
         runner = (LuaAllocator::Freelist*)runner->nextFreeAddress;
      }
      u8* nextAddress = runner->nextFreeAddress;
      u8* freeAddress = (u8*)ptr;
      if( freeAddress + size == nextAddress )
      {
         //combine
         LuaAllocator::Freelist* nextRunner = (LuaAllocator::Freelist*)nextAddress;
         runner->freeSize += size + nextRunner->freeSize;
         runner->nextFreeAddress = nextRunner->nextFreeAddress;
      }
      else
      {
         //fragment
      }
      runner->nextFreeAddress = (u8*)ptr;
      runner->freeSize += size;
      
   }


   void* lua_allocateCustom(void* ud, void* ptr, size_t osize, size_t nsize)
   {
      LuaAllocator* a = (LuaAllocator*)ud;
      void* result = nullptr;
      if( nsize == 0 )
      {
         deallocate(*a, ptr, (u32)osize);
      }
      else if( osize == 0 && nsize > 0 )
      {
         result = allocate(*a, (u32)nsize);
      }
      else if( osize > 0 && nsize > 0 )
      {
         result = reallocate(*a, ptr, (u32)osize, (u32)nsize);
      }
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
      
      return realloc(ptr, nsize);
   }

   bool LuaSystem::init(LinearAllocator& a, u32 memorySize)
   {
      CORE_INIT_START(LuaSystem);

      m_allocator.size = memorySize;
      m_allocator.tag = "Lua system";
      m_allocator.allocated = 0;
      m_allocator.memory = allocate(a, m_allocator.size, 1);

      m_L = lua_newstate(lua_allocate, &m_allocator);
      
      CORE_STATUS_AND(m_L != nullptr);
      if( CORE_STATUS_OK )
      {
         luaL_openlibs(m_L);
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
}
