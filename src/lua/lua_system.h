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
      bool init();
      bool shutdown();
      void collectGarbage();

      LuaStack getStack() const;

   private:
      lua_State* m_L;
      LinearAllocator m_staticMemory;
      HeapAllocator m_luaMemory;

      friend LuaSystem* createScriptSystem(MemoryBlock memory);
   };

   LuaSystem* createScriptSystem(MemoryBlock memory);

   void test_luaStack();
}
