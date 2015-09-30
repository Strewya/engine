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
#include "lua/lua_stack.h"
#include "utility/memory.h"
/******* end header inclusion *******/

namespace core
{
   struct LuaSystem
   {
   public:
      void init(Memory a);
      void shutdown();
      void collectGarbage();

      LuaStack getStack() const;

   private:
      lua_State* m_L;
      Memory m_luaMemory;
   };

   void test_luaStack();
}
