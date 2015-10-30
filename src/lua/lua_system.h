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
      bool runFile(LuaSystem* lua, str file)
      {
         bool result = luaL_dofile(lua->m_L, file) == 0;
         return result;
      }

      u32 readValue(LuaSystem* lua, str key, u32 defaultValue)
      {
         u32 top = lua_gettop(lua->m_L);
         lua_pushstring(lua->m_L, key);
         lua_gettable(lua->m_L, -2);
         if( lua_isnumber(lua->m_L, -1) )
         {
            u32 result = (u32)lua_tonumber(lua->m_L, -1);
            lua_pop(lua->m_L, 1);
            return result;
         }
         lua_settop(lua->m_L, top);
         return defaultValue;
      }
   }
}
