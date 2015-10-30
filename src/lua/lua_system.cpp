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

   void* lua_allocateCustom(void* ud, void* ptr, size_t osize, size_t nsize)
   {
      CORE_LOG_DEBUG("lua_allocateCustom, ptr/osize/nsize: ", memoryAddress(ptr), "/", osize, "/", nsize);
      auto* memory = (Memory*)ud;
      void* result = nullptr;
      if( nsize == 0 )
      {
         //a->deallocateRaw(ptr, (u32)osize);
      }
      else if( osize == 0 && nsize > 0 )
      {
         auto before = __rdtsc();
         //result = a->allocateRaw((u32)nsize, 16);
         auto after = __rdtsc();
         CORE_LOG_DEBUG("allocateRaw took ", (after - before), " cycles");
      }
      else if( osize > 0 && nsize > 0 )
      {
         if( osize != nsize )
         {
            auto before = __rdtsc();
            //result = a->allocateRaw((u32)nsize, 16);
            memcpy(result, ptr, min(osize, nsize));
            //a->deallocateRaw(ptr, (u32)osize);
            auto after = __rdtsc();
            CORE_LOG_DEBUG("allocateRaw + memcpy + deallocateRaw took ", (after - before), " cycles");
         }
         else
         {
            result = ptr;
         }
      }

      return result;
   }

   void* lua_allocate(void* ud, void* ptr, size_t osize, size_t nsize)
   {
      //CORE_LOG_DEBUG("lua_allocate, ptr/osize/nsize: ", memoryAddress(ptr), "/", osize, "/", nsize);

      (void)ud;  (void)osize;  /* not used */
      if( nsize == 0 )
      {
         free(ptr);
         return nullptr;
      }
      auto before = __rdtsc();
      auto* result = realloc(ptr, nsize);
      auto after = __rdtsc();
      //CORE_LOG_DEBUG("realloc took ", (after - before), " cycles");
      return result;
   }

   void LuaSystem::init(Memory a)
   {
      m_luaMemory = a;
      m_L = lua_newstate(lua_allocate, &m_luaMemory);

      CORE_ASSERT_DBGERR(m_L != nullptr, "Failed to create Lua state!");
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

      auto result = luaL_dostring(m_L, depend);
      CORE_ASSERT_DBGERR(result == 0, "Failed to register dependency code to Lua!");
   }

   void LuaSystem::shutdown()
   {
      lua_close(m_L);
   }

   void LuaSystem::collectGarbage()
   {
      lua_gc(m_L, LUA_GCSTEP, 20);
   }
}
