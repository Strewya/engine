//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "lua/lua_system.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "utility/utility.h"
/******* end headers *******/

namespace core
{
   void LuaStack::dumpStack()
   {
      u32 n = lua_gettop(m_L);
      for( u32 i = 1; i <= n; ++i )
      {
         switch( lua_type(m_L, i) )
         {
            case LUA_TNUMBER:
            {
               CORE_LOG_DEBUG("[", i, "]=", lua_tonumber(m_L, i));
            } break;
            case LUA_TSTRING:
            {
               CORE_LOG_DEBUG("[", i, "]=", lua_tostring(m_L, i));
            } break;
            case LUA_TBOOLEAN:
            {
               CORE_LOG_DEBUG("[", i, "]=", lua_toboolean(m_L, i) == 1 ? "true" : "false");
            } break;
            default:
            {
               CORE_LOG_DEBUG("[", i, "]=", lua_typename(m_L, i), "(", lua_tonumber(m_L, i), ")");
            } break;
         }
      }
      CORE_LOG_DEBUG("-------------------");
   }

   void test_luaStack()
   {
      
   }
}