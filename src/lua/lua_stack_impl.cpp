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
   template<> std::string LuaStack::to<std::string>(int32_t stackIndex)
   {
      return lua_tostring(m_L, stackIndex);
   }

   template<> uint32_t LuaStack::to<uint32_t>(int32_t stackIndex)
   {
      return lua_tointeger(m_L, stackIndex);
   }

   template<> int32_t LuaStack::to<int32_t>(int32_t stackIndex)
   {
      return lua_tointeger(m_L, stackIndex);
   }

   template<> float LuaStack::to<float>(int32_t stackIndex)
   {
      return (float)lua_tonumber(m_L, stackIndex);
   }

   template<> double LuaStack::to<double>(int32_t stackIndex)
   {
      return (double)lua_tonumber(m_L, stackIndex);
   }

   template<> bool LuaStack::to<bool>(int32_t stackIndex)
   {
      return lua_toboolean(m_L, stackIndex) == 1;
   }



   template<> bool LuaStack::is<nullptr_t>(int32_t stackIndex)
   {
      return lua_isnil(m_L, stackIndex);
   }

   template<> bool LuaStack::is<bool>(int32_t stackIndex)
   {
      return lua_isboolean(m_L, stackIndex);
   }

   template<> bool LuaStack::is<int32_t>(int32_t stackIndex)
   {
      return lua_isnumber(m_L, stackIndex) == 1;
   }

   template<> bool LuaStack::is<uint32_t>(int32_t stackIndex)
   {
      return lua_isnumber(m_L, stackIndex) == 1;
   }

   template<> bool LuaStack::is<float>(int32_t stackIndex)
   {
      return lua_isnumber(m_L, stackIndex) == 1;
   }

   template<> bool LuaStack::is<double>(int32_t stackIndex)
   {
      return lua_isnumber(m_L, stackIndex) == 1;
   }

   template<> bool LuaStack::is<std::string>(int32_t stackIndex)
   {
      return (lua_isstring(m_L, stackIndex) == 1);
   }

   template<> bool LuaStack::is<LuaTable>(int32_t stackIndex)
   {
      return lua_istable(m_L, stackIndex);
   }

   template<> bool LuaStack::is<LuaFunction>(int32_t stackIndex)
   {
      return lua_isfunction(m_L, stackIndex);
   }


   template<> void LuaStack::push<std::string>(std::string arg)
   {
      lua_pushstring(m_L, arg.c_str());
   }

   template<> void LuaStack::push<int32_t>(int32_t arg)
   {
      lua_pushinteger(m_L, arg);
   }

   template<> void LuaStack::push<uint32_t>(uint32_t arg)
   {
      lua_pushinteger(m_L, arg);
   }

   template<> void LuaStack::push<bool>(bool arg)
   {
      lua_pushboolean(m_L, arg);
   }

   template<> void LuaStack::push<float>(float arg)
   {
      lua_pushnumber(m_L, arg);
   }

   template<> void LuaStack::push<double>(double arg)
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

   template<> char get<char>(LuaStack& lua, int32_t stackIndex, char valueIfMissing)
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
