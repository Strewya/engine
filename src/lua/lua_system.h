#pragma once
/********************************************
*  contents:   #description
*  usage:
********************************************/
/******* c++ headers *******/
#include <cstdint>
/******* common headers *******/
#include "lua/lua_include.h"
/******* extra headers *******/
#include "lua/lua_stack.h"
/******* end header inclusion *******/

namespace core
{
   struct LuaSystem
   {
   public:
      bool init();
      bool shutdown();
      void collectGarbage();

      LuaStack getStack() const;


   private:
      lua_State* m_L;
   };

   void test_luaStack();
}
