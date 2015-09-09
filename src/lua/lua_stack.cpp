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
      : m_L(L), m_narg(0), m_nres(0), m_freeIterationSlot(0)
   {
   }

   bool LuaStack::loadFile(const char* file)
   {
      auto result = (luaL_loadfile(m_L, file) == LUA_OK);
      return result;
   }

   bool LuaStack::doFile(const char* file)
   {
      auto result = (loadFile(file) && call());
      return result;
   }

   u32 LuaStack::getTop()
   {
      auto result = lua_gettop(m_L);
      return result;
   }

   void LuaStack::pop(i32 howMany)
   {
      lua_pop(m_L, howMany);
   }

   void LuaStack::pull(u32 key, i32 stackIndex)
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

   void LuaStack::pull(const char* key, i32 stackIndex)
   {
      stackIndex = stackIndex == 0 ? LUA_GLOBALSINDEX : stackIndex;
      lua_getfield(m_L, stackIndex, key);
   }

   void LuaStack::pairs(i32 stackIndex)
   {
      CORE_ASSERT(AssertLevel::Fatal, m_freeIterationSlot < MaxIterations);
      m_iters[m_freeIterationSlot++] = Iteration{stackIndex < 0 ? getTop() + 1 + stackIndex : stackIndex, true, false};
      if( is<LuaTable>(m_iters[m_freeIterationSlot - 1].m_iterateTableIndex) )
      {
         lua_pushnil(m_L);
      }
   }

   void LuaStack::pairs(const char* table)
   {
      CORE_ASSERT(AssertLevel::Fatal, m_freeIterationSlot < MaxIterations);
      pull(table);
      m_iters[m_freeIterationSlot++] = Iteration{getTop(), true, true};
      if( is<LuaTable>() )
      {
         lua_pushnil(m_L);
      }
   }

   void LuaStack::ipairs(i32 stackIndex)
   {
      pairs(stackIndex);
      m_iters[m_freeIterationSlot - 1].m_iterateAll = false;
   }

   void LuaStack::ipairs(const char* table)
   {
      pairs(table);
      m_iters[m_freeIterationSlot - 1].m_iterateAll = false;
   }

   bool LuaStack::next()
   {
      i32 res = 0;
      if( is<LuaTable>(m_iters[m_freeIterationSlot - 1].m_iterateTableIndex) )
      {
         res = lua_next(m_L, m_iters[m_freeIterationSlot - 1].m_iterateTableIndex);
         if( res != 0 )
         {
            if( !m_iters[m_freeIterationSlot - 1].m_iterateAll && !is<u32>(-2) )
            {
               do
               {
                  pop();
                  res = lua_next(m_L, m_iters[m_freeIterationSlot - 1].m_iterateTableIndex);
               } while( res != 0 && !is<u32>(-2) );
            }
         }
      }
      if( res == 0 )
      {
         if( m_iters[m_freeIterationSlot - 1].m_autoPopTable )
         {
            pop();
         }
         --m_freeIterationSlot;
      }
      auto result = res != 0;
      return result;
   }

   void LuaStack::setValue(const char* key, i32 stackIndex)
   {
      stackIndex = stackIndex == 0 ? LUA_GLOBALSINDEX : stackIndex;
      if( is<LuaTable>(stackIndex) )
      {
         lua_setfield(m_L, stackIndex, key);
      }
   }

   bool LuaStack::call()
   {
      auto r = (lua_pcall(m_L, m_narg, (m_nres == 0 ? LUA_MULTRET : m_nres), 0) == LUA_OK);
      return r;
   }
}
