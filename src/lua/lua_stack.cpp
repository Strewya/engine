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
   LuaStack::LuaStack(lua_State* L)
      : m_L(L), m_narg(0), m_nres(0)
   {
   }

   bool LuaStack::loadFile(const char* file)
   {
      return (luaL_loadfile(m_L, file) == LUA_OK);
   }

   bool LuaStack::doFile(const char* file)
   {
      return (loadFile(file) && call());
   }

   uint32_t LuaStack::getTop()
   {
      return lua_gettop(m_L);
   }

   void LuaStack::pop(int32_t howMany)
   {
      lua_pop(m_L, howMany);
   }

   void LuaStack::pull(uint32_t key, int32_t stackIndex)
   {
      if( stackIndex < 0 )
      {
         --stackIndex;
      }
      else if( stackIndex == 0 )
      {
         stackIndex = LUA_GLOBALSINDEX;
      }
      lua_pushinteger(m_L, key);
      lua_gettable(m_L, stackIndex);
   }

   void LuaStack::pull(const std::string& key, int32_t stackIndex)
   {
      stackIndex = stackIndex == 0 ? LUA_GLOBALSINDEX : stackIndex;
      lua_getfield(m_L, stackIndex, key.c_str());
   }

   void LuaStack::pairs(int32_t stackIndex)
   {
      m_iters.emplace_back(Iteration{stackIndex < 0 ? getTop() + 1 + stackIndex : stackIndex, true, false});
      if( is<LuaTable>(m_iters.back().m_iterateTableIndex) )
      {
         lua_pushnil(m_L);
      }
   }

   void LuaStack::pairs(const std::string& table)
   {
      pull(table);
      m_iters.emplace_back(Iteration{getTop(), true, true});
      if( is<LuaTable>() )
      {
         lua_pushnil(m_L);
      }
   }

   void LuaStack::ipairs(int32_t stackIndex)
   {
      pairs(stackIndex);
      m_iters.back().m_iterateAll = false;
   }

   void LuaStack::ipairs(const std::string& table)
   {
      pairs(table);
      m_iters.back().m_iterateAll = false;
   }

   bool LuaStack::next()
   {
      int32_t res = 0;
      if( is<LuaTable>(m_iters.back().m_iterateTableIndex) )
      {
         res = lua_next(m_L, m_iters.back().m_iterateTableIndex);
         if( res != 0 )
         {
            if( !m_iters.back().m_iterateAll && !is<uint32_t>(-2) )
            {
               do
               {
                  pop();
                  res = lua_next(m_L, m_iters.back().m_iterateTableIndex);
               } while( res != 0 && !is<uint32_t>(-2) );
            }
         }
      }
      if( res == 0 )
      {
         if( m_iters.back().m_autoPopTable )
         {
            pop();
         }
         m_iters.pop_back();
      }
      return res != 0;
   }

   void LuaStack::setValue(const std::string& key, int32_t stackIndex)
   {
      stackIndex = stackIndex == 0 ? LUA_GLOBALSINDEX : stackIndex;
      if( is<LuaTable>(stackIndex) )
      {
         lua_setfield(m_L, stackIndex, key.c_str());
      }
   }

   bool LuaStack::call()
   {
      auto r = (lua_pcall(m_L, m_narg, (m_nres == 0 ? LUA_MULTRET : m_nres), 0) == LUA_OK);
      return r;
   }
}
