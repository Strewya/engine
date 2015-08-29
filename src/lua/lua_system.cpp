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
   int luaPrint(lua_State* L)
   {
      i32 n = lua_gettop(L);
      for( i32 i = 1; i <= n; ++i )
      {
         switch( lua_type(L, i) )
         {
            case LUA_TNUMBER:
               std::cout << lua_tonumber(L, i);
               break;
            case LUA_TSTRING:
               std::cout << lua_tostring(L, i);
               break;
            case LUA_TBOOLEAN:
               std::cout << (lua_toboolean(L, i) == 1 ? "true" : "false");
               break;
            default:
               std::cout << lua_typename(L, i) << "(" << lua_tonumber(L, i) << ")";
               break;
         }
      }
      std::cout << std::endl;
      return 0;
   }

   int luaErrHandler(lua_State* L)
   {
      lua_pushnil(L);
      return 2;
   }

   bool LuaSystem::init()
   {
      CORE_INIT_START(LuaSystem);
      CORE_STATUS_AND(false);
      m_L = luaL_newstate();
      if( m_L )
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

         CORE_STATUS_OR(luaL_dostring(m_L, depend) == 0);
      }

      CORE_INIT_END(LuaSystem);
   }

   bool LuaSystem::shutdown()
   {
      CORE_SHUTDOWN_START(LuaSystem);
      lua_close(m_L);
      CORE_SHUTDOWN_END(LuaSystem);
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
