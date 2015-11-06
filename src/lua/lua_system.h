#pragma once
/********************************************
*  contents:   #description
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   struct LuaSystem;
   struct Memory;

   LuaSystem* initLuaSystem(Memory memBlock);
   void shutdown(LuaSystem* lua);

   void frameUpdate(LuaSystem* lua);

   namespace script
   {
      struct LuaTable
      {
         i32 index;
      };

      LuaTable getGlobalNamespace(LuaSystem* L);
      
      LuaTable beginNewNamespace(LuaSystem* L, str k);
      void endNewNamespace(LuaSystem* L, LuaTable t);
      
      LuaTable openNamespace(LuaSystem* L, LuaTable t, str k);
      void closeNamespace(LuaSystem* L, LuaTable t);
      
      LuaTable openConfigFile(LuaSystem* L, str file);
      void closeConfigFile(LuaSystem* L, LuaTable file);
      
      u32 readNamedValue(LuaSystem* L, LuaTable t, str k, u32 valueIfMissing);
      str readIndexedValue(LuaSystem* L, LuaTable t, u32 k, str valueIfMissing, char* resultBuffer);
      
      void writeNamedValue(LuaSystem* L, LuaTable t, str k, u32 v);
   }
}
