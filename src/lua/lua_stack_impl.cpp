//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "lua/lua_stack.h"
/******* c++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace core
{
   template<> std::string LuaStack::to<std::string>(i32 stackIndex)
   {
      return lua_tostring(m_L, stackIndex);
   }

   template<> u32 LuaStack::to<u32>(i32 stackIndex)
   {
      return (u32)lua_tointeger(m_L, stackIndex);
   }

   template<> i32 LuaStack::to<i32>(i32 stackIndex)
   {
      return (i32)lua_tointeger(m_L, stackIndex);
   }

   template<> f32 LuaStack::to<f32>(i32 stackIndex)
   {
      return (f32)lua_tonumber(m_L, stackIndex);
   }

   template<> f64 LuaStack::to<f64>(i32 stackIndex)
   {
      return (f64)lua_tonumber(m_L, stackIndex);
   }

   template<> bool LuaStack::to<bool>(i32 stackIndex)
   {
      return lua_toboolean(m_L, stackIndex) == 1;
   }



   template<> bool LuaStack::is<nullptr_t>(i32 stackIndex)
   {
      return lua_isnil(m_L, stackIndex);
   }

   template<> bool LuaStack::is<bool>(i32 stackIndex)
   {
      return lua_isboolean(m_L, stackIndex);
   }

   template<> bool LuaStack::is<i32>(i32 stackIndex)
   {
      return lua_isnumber(m_L, stackIndex) == 1;
   }

   template<> bool LuaStack::is<u32>(i32 stackIndex)
   {
      return lua_isnumber(m_L, stackIndex) == 1;
   }

   template<> bool LuaStack::is<f32>(i32 stackIndex)
   {
      return lua_isnumber(m_L, stackIndex) == 1;
   }

   template<> bool LuaStack::is<f64>(i32 stackIndex)
   {
      return lua_isnumber(m_L, stackIndex) == 1;
   }

   template<> bool LuaStack::is<std::string>(i32 stackIndex)
   {
      return (lua_isstring(m_L, stackIndex) == 1);
   }

   template<> bool LuaStack::is<LuaTable>(i32 stackIndex)
   {
      return lua_istable(m_L, stackIndex);
   }

   template<> bool LuaStack::is<LuaFunction>(i32 stackIndex)
   {
      return lua_isfunction(m_L, stackIndex);
   }


   template<> void LuaStack::push<std::string>(std::string arg)
   {
      lua_pushstring(m_L, arg.c_str());
   }

   template<> void LuaStack::push<i32>(i32 arg)
   {
      lua_pushinteger(m_L, arg);
   }

   template<> void LuaStack::push<u32>(u32 arg)
   {
      lua_pushinteger(m_L, arg);
   }

   template<> void LuaStack::push<bool>(bool arg)
   {
      lua_pushboolean(m_L, arg);
   }

   template<> void LuaStack::push<f32>(f32 arg)
   {
      lua_pushnumber(m_L, arg);
   }

   template<> void LuaStack::push<f64>(f64 arg)
   {
      lua_pushnumber(m_L, arg);
   }

   template<> void LuaStack::push<nullptr_t>(nullptr_t arg)
   {
      lua_pushnil(m_L);
   }

   template<> void LuaStack::push<LuaCustom>(LuaCustom arg)
   {
      tolua_pushusertype(m_L, arg.ptr, arg.name);
   }


   template<> char get<char>(LuaStack& lua, const std::string& id, char valueIfMissing)
   {
      lua.pull(id);
      if( lua.is<std::string>() )
      {
         valueIfMissing = lua.to<std::string>().at(0);
      }
      lua.pop();
      return valueIfMissing;
   }

   template<> char get<char>(LuaStack& lua, i32 stackIndex, char valueIfMissing)
   {
      lua.pull(stackIndex);
      if( lua.is<std::string>() )
      {
         valueIfMissing = lua.to<std::string>().at(0);
      }
      lua.pop();
      return valueIfMissing;
   }
}
