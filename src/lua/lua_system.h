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
#include "utility/memory.h"
/******* end header inclusion *******/

namespace core
{
   struct LuaSystem
   {
      void init(Memory a);
      void shutdown();
      void collectGarbage();

      lua_State* m_L;
      Memory m_luaMemory;
   };


   namespace script
   {
      inline int luaPrint(lua_State* L);


#define Lua L->m_L

      struct LuaTable
      {
         i32 index;
      };

      inline LuaTable getGlobalNamespace(LuaSystem* L)
      {
         LuaTable result{LUA_GLOBALSINDEX};
         return result;
      }

      inline LuaTable beginNewNamespace(LuaSystem* L, str k)
      {
         luaPrint(Lua);
         lua_pushstring(Lua, k);
         luaPrint(Lua);
         lua_createtable(Lua, 0, 0);
         luaPrint(Lua);
         LuaTable result{lua_gettop(Lua)};
         return result;
      }

      inline void endNewNamespace(LuaSystem* L, LuaTable t)
      {
         //assumption is that somebody called startNewNamespace first, placing a string and then a table onto the stack
         //we now make sure that the top contains a table, the slot beneath it contains a string and the target table is a valid table
         luaPrint(Lua);
         auto top = lua_gettop(Lua);
         if( !lua_istable(Lua, -1) )
         {
            //error, top of the stack does not contain a table
            return;
         }

         if( !lua_isstring(Lua, -2) )
         {
            //error, the slot beneath the top is not a string
            return;
         }

         if( !lua_istable(Lua, t.index) )
         {
            //error, target table index does not contain a table
            return;
         }

         lua_settable(Lua, t.index);
         luaPrint(Lua);
         CORE_ASSERT_DBGERR(top - 2 == lua_gettop(Lua), "Unexpected number of values on Lua stack! Expected ", top - 2, ", have ", lua_gettop(Lua));
      }

      inline LuaTable openNamespace(LuaSystem* L, LuaTable t, str k)
      {
         luaPrint(Lua);
         auto top = lua_gettop(Lua);
         LuaTable result{0};

         if( !lua_istable(Lua, t.index) )
         {
            //error, source table index is not actually a table
            return result;
         }

         lua_getfield(Lua, t.index, k);
         luaPrint(Lua);
         if( !lua_istable(Lua, -1) )
         {
            //error, the key does not contain a table
            lua_settop(Lua, top);
            return result;
         }

         result.index = lua_gettop(Lua);
         return result;
      }

      inline void closeNamespace(LuaSystem* L, LuaTable t)
      {
         luaPrint(Lua);
         auto top = lua_gettop(Lua);
         if( top != t.index )
         {
            //error, table is not at top of stack
            //is this really an error? i could just set the top to be t.index-1...
            return;
         }

         lua_pop(Lua, 1);
         luaPrint(Lua);
      }

      inline LuaTable openConfigFile(LuaSystem* L, str file)
      {
         luaPrint(Lua);
         auto top = lua_gettop(Lua);
         auto result = LuaTable{0};
         auto loadedAndExecuted = luaL_dofile(Lua, file) == 0;
         luaPrint(Lua);
         if( loadedAndExecuted )
         {
            if( lua_istable(Lua, -1) )
            {
               result.index = lua_gettop(Lua);
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
         luaPrint(Lua);
         return result;
      }

      inline void closeConfigFile(LuaSystem* L, LuaTable file)
      {
         luaPrint(Lua);
         if( lua_istable(Lua, file.index) )
         {
            lua_pop(Lua, 1);
         }
         luaPrint(Lua);
      }

      inline u32 readNamedValue(LuaSystem* L, LuaTable t, str k, u32 valueIfMissing)
      {
         luaPrint(Lua);
         auto top = lua_gettop(Lua);
         if( !lua_istable(Lua, t.index) )
         {
            //error
            return valueIfMissing;
         }

         lua_getfield(Lua, t.index, k);
         luaPrint(Lua);
         if( !lua_isnumber(Lua, -1) )
         {
            //hm, not what we expected...
            return valueIfMissing;
         }

         auto result = (u32)lua_tonumber(Lua, -1);

         lua_pop(Lua, 1);
         CORE_ASSERT_DBGERR(top == lua_gettop(Lua), "Unexpected number of values on Lua stack! Expected ", top + 1, ", have ", lua_gettop(Lua));
         luaPrint(Lua);
         return result;
      }

      inline str readIndexedValue(LuaSystem* L, LuaTable t, u32 k, str valueIfMissing, Memory resultBuffer)
      {
         luaPrint(Lua);
         auto top = lua_gettop(Lua);
         if( !lua_istable(Lua, t.index) )
         {
            //error
            return valueIfMissing;
         }

         lua_pushnumber(Lua, k);
         luaPrint(Lua);
         lua_gettable(Lua, t.index);
         luaPrint(Lua);

         if( !lua_isstring(Lua, -1) )
         {
            //error
            return valueIfMissing;
         }
         u32 l = (u32)lua_objlen(Lua, -1);
         char* result = emplaceArray<char>(resultBuffer, l + 1);
         str v = (str)lua_tostring(Lua, -1);
         std::copy(v, v + l, result);
         lua_pop(Lua, 1);
         luaPrint(Lua);
         return result;
      }

      inline void writeNamedValue(LuaSystem* L, LuaTable t, str k, u32 v)
      {
         luaPrint(Lua);
         auto top = lua_gettop(Lua);
         if( !lua_istable(Lua, t.index) )
         {
            //error
            return;
         }

         lua_pushnumber(Lua, v);
         luaPrint(Lua);
         lua_setfield(Lua, t.index, k);
         luaPrint(Lua);
         CORE_ASSERT_DBGERR(top == lua_gettop(Lua), "Unexpected number of values on Lua stack! Expected ", top + 1, ", have ", lua_gettop(Lua));
      }



#if 0
      u32 openConfigFile(LuaSystem* L, str file)
      {
         u32 top = lua_gettop(Lua);
         auto result = 0U;
         bool ok = luaL_dofile(Lua, file) == 0;
         if( ok && lua_istable(Lua, -1) )
         {
            result = lua_gettop(Lua);
            CORE_ASSERT_DBGERR(result == (top + 1), "Unexpected number of values on stack! Expected ", top + 1, ", have ", result);
         }
         else
         {
            //error handling, pop string off ? not sure if this generates an error string on the stack
         }
         return result;
      }

      void closeConfigFile(LuaSystem* L, u32 table)
      {
         if( lua_istable(Lua, table) )
         {
            lua_pop(Lua, 1);
         }
      }

      u32 readNamedValueFromTable(LuaSystem* L, u32 table, str key, u32 defaultValue)
      {
         u32 oldTop = lua_gettop(Lua);
         lua_pushstring(Lua, key);
         lua_gettable(Lua, table);
         if( (oldTop + 1) == lua_gettop(Lua) && lua_isnumber(Lua, -1) )
         {
            u32 result = (u32)lua_tonumber(Lua, -1);
            lua_pop(Lua, 1);
            return result;
         }
         else
         {
            //error?
         }
         lua_settop(Lua, oldTop);
         return defaultValue;
      }

      void writeNamedValueToTable(LuaSystem* L, u32 table, str key, u32 value)
      {
         lua_pushstring(Lua, key);
         lua_pushnumber(Lua, value);
         lua_settable(Lua, table);
      }

      u32 createTable(LuaSystem* L)
      {
         lua_newtable(Lua);
         auto result = lua_gettop(Lua);
         return result;
      }

      u32 createPreallocatedTable(LuaSystem* L, u32 arraySize, u32 tableSize)
      {
         lua_createtable(Lua, arraySize, tableSize);
         auto result = lua_gettop(Lua);
         return result;
      }

      void setGlobalValue(LuaSystem* L)
      {

      }
#endif
#undef Lua
   }
}
