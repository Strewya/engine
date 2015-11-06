//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "lua/lua_system.h"
/******* c++ headers *******/
#include <algorithm>
#include <cstdio>
/******* extra headers *******/
#include "lua/lua_include.h"
#include "utility/memory.h"
#include "utility/utility.h"
/******* end headers *******/

namespace core
{
   struct LuaSystem
   {
      lua_State* luaState;
      Memory memory;
   };

   int luaPrint(lua_State* L)
   {
      i32 n = lua_gettop(L);
      if( n == 0 )
      {
         CORE_LOG_DEBUG("Empty");
         return 0;
      }
      for( i32 i = 1; i <= n; ++i )
      {
         auto t = lua_type(L, i);
         switch( t )
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
               CORE_LOG_DEBUG(lua_typename(L, t), "(", lua_tonumber(L, i), ")");
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

   LuaSystem* initLuaSystem(Memory a)
   {
      auto* lua = emplace<LuaSystem>(a);
      if( !lua )
      {
         CORE_LOG("Not enough memory for Lua subsystem!");
         return nullptr;
      }

      lua->memory = a;
      lua->luaState = lua_newstate(lua_allocate, &lua->memory);
      
      if(!lua->luaState)
      {
         CORE_LOG("Failed to create Lua state!");
         return nullptr;
      }
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
      luaL_openlibs(lua->luaState);
#endif

      //tolua_core_open(m_L);
      lua_register(lua->luaState, "print", luaPrint);

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

      auto result = luaL_dostring(lua->luaState, depend);
      if( result != 0 )
      {
         CORE_LOG("Failed to register dependency code to Lua!");
      }

      return lua;
   }

   void shutdown(LuaSystem* lua)
   {
      lua_close(lua->luaState);
   }

   void frameUpdate(LuaSystem* lua)
   {
      lua_gc(lua->luaState, LUA_GCSTEP, 20);
   }

   namespace script
   {
      LuaTable getGlobalNamespace(LuaSystem* L)
      {
         LuaTable result{LUA_GLOBALSINDEX};
         return result;
      }

      LuaTable beginNewNamespace(LuaSystem* lua, str k)
      {
         auto L = lua->luaState;
         luaPrint(L);
         lua_pushstring(L, k);
         luaPrint(L);
         lua_createtable(L, 0, 0);
         luaPrint(L);
         LuaTable result{lua_gettop(L)};
         return result;
      }

      void endNewNamespace(LuaSystem* lua, LuaTable t)
      {
         auto L = lua->luaState;

         //assumption is that somebody called startNewNamespace first, placing a string and then a table onto the stack
         //we now make sure that the top contains a table, the slot beneath it contains a string and the target table is a valid table
         luaPrint(L);
         auto top = lua_gettop(L);
         if( !lua_istable(L, -1) )
         {
            //error, top of the stack does not contain a table
            return;
         }

         if( !lua_isstring(L, -2) )
         {
            //error, the slot beneath the top is not a string
            return;
         }

         if( !lua_istable(L, t.index) )
         {
            //error, target table index does not contain a table
            return;
         }

         lua_settable(L, t.index);
         luaPrint(L);
         CORE_ASSERT_DBGERR(top - 2 == lua_gettop(L), "Unexpected number of values on Lua stack! Expected ", top - 2, ", have ", lua_gettop(L));
      }

      LuaTable openNamespace(LuaSystem* lua, LuaTable t, str k)
      {
         auto L = lua->luaState;

         luaPrint(L);
         auto top = lua_gettop(L);
         LuaTable result{0};

         if( !lua_istable(L, t.index) )
         {
            //error, source table index is not actually a table
            return result;
         }

         lua_getfield(L, t.index, k);
         luaPrint(L);
         if( !lua_istable(L, -1) )
         {
            //error, the key does not contain a table
            lua_settop(L, top);
            return result;
         }

         result.index = lua_gettop(L);
         return result;
      }

      void closeNamespace(LuaSystem* lua, LuaTable t)
      {
         auto L = lua->luaState;

         luaPrint(L);
         auto top = lua_gettop(L);
         if( top != t.index )
         {
            //error, table is not at top of stack
            //is this really an error? i could just set the top to be t.index-1...
            return;
         }

         lua_pop(L, 1);
         luaPrint(L);
      }

      LuaTable openConfigFile(LuaSystem* lua, str file)
      {
         auto L = lua->luaState;

         luaPrint(L);
         auto top = lua_gettop(L);
         auto result = LuaTable{0};
         auto loadedAndExecuted = luaL_dofile(L, file) == 0;
         luaPrint(L);
         if( loadedAndExecuted )
         {
            if( lua_istable(L, -1) )
            {
               result.index = lua_gettop(L);
               CORE_ASSERT_DBGERR(result.index == (top + 1), "Unexpected number of values on stack! Expected ", top + 1, ", have ", result.index);
            }
            else
            {
               //invalid config file structure
            }
         }
         else
         {
            //failed to load and execute
         }
         luaPrint(L);
         return result;
      }

      void closeConfigFile(LuaSystem* lua, LuaTable file)
      {
         auto L = lua->luaState;

         luaPrint(L);
         if( lua_istable(L, file.index) )
         {
            lua_pop(L, 1);
         }
         luaPrint(L);
      }

      u32 readNamedValue(LuaSystem* lua, LuaTable t, str k, u32 valueIfMissing)
      {
         auto L = lua->luaState;

         luaPrint(L);
         auto top = lua_gettop(L);
         if( !lua_istable(L, t.index) )
         {
            //error
            return valueIfMissing;
         }

         lua_getfield(L, t.index, k);
         luaPrint(L);
         if( !lua_isnumber(L, -1) )
         {
            //hm, not what we expected...
            return valueIfMissing;
         }

         auto result = (u32)lua_tonumber(L, -1);

         lua_pop(L, 1);
         CORE_ASSERT_DBGERR(top == lua_gettop(L), "Unexpected number of values on Lua stack! Expected ", top + 1, ", have ", lua_gettop(L));
         luaPrint(L);
         return result;
      }

      str readIndexedValue(LuaSystem* lua, LuaTable t, u32 k, str valueIfMissing, char* resultBuffer)
      {
         auto L = lua->luaState;

         luaPrint(L);
         auto top = lua_gettop(L);
         if( !lua_istable(L, t.index) )
         {
            //error
            return valueIfMissing;
         }

         lua_pushnumber(L, k);
         luaPrint(L);
         lua_gettable(L, t.index);
         luaPrint(L);

         if( !lua_isstring(L, -1) )
         {
            //error
            return valueIfMissing;
         }
         u32 l = (u32)lua_objlen(L, -1);
         str v = (str)lua_tostring(L, -1);
         std::copy(v, v + l + 1, resultBuffer);
         lua_pop(L, 1);
         luaPrint(L);
         return resultBuffer;
      }

      void writeNamedValue(LuaSystem* lua, LuaTable t, str k, u32 v)
      {
         auto L = lua->luaState;

         luaPrint(L);
         auto top = lua_gettop(L);
         if( !lua_istable(L, t.index) )
         {
            //error
            return;
         }

         lua_pushnumber(L, v);
         luaPrint(L);
         lua_setfield(L, t.index, k);
         luaPrint(L);
         CORE_ASSERT_DBGERR(top == lua_gettop(L), "Unexpected number of values on Lua stack! Expected ", top + 1, ", have ", lua_gettop(L));
      }
   }
}
