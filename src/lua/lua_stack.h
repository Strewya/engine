#pragma once
/********************************************
*  contents:   #description
*  usage:
********************************************/
/******* c++ headers *******/
#include <string>
#include <vector>
/******* common headers *******/
#include "lua/lua_include.h"
#include "utility/types.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   struct LuaCustom
   {
      void* ptr;
      const char* name;
   };

   struct LuaTable
   {
   };

   struct LuaFunction
   {
   };

   struct LuaStack
   {
   public:
      LuaStack(lua_State* L);
      void dumpStack();

      bool loadFile(const char* file);
      bool doFile(const char* file);

      u32 getTop();
      void pop(i32 howMany = 1);
      void pull(u32 key, i32 stackIndex = -1);
      void pull(const std::string& key, i32 stackIndex = -1);

      void pairs(i32 stackIndex = -1);
      void pairs(const std::string& table);
      void ipairs(i32 stackIndex = -1);
      void ipairs(const std::string& table);

      bool next();

      void setValue(const std::string& key, i32 stackIndex = -1);

      template<typename T> void push(T arg);

      template<typename T> T to(i32 stackIndex = -1);

      template<typename T> bool is(i32 stackIndex = -1);

      bool call();
      template<typename T, typename ...Args> bool call(T&& arg, Args... rest);
      template<typename T, typename ...Args> bool call(T* arg, Args... rest);

   private:
      lua_State* m_L;
      u32 m_narg;
      u32 m_nres;
      struct Iteration
      {
         i32 m_iterateTableIndex;
         bool m_iterateAll;
         bool m_autoPopTable;
      };
      std::vector<Iteration> m_iters;
   };

   template<> void LuaStack::push<std::string>(std::string arg);
   template<> void LuaStack::push<i32>(i32 arg);
   template<> void LuaStack::push<u32>(u32 arg);
   template<> void LuaStack::push<bool>(bool arg);
   template<> void LuaStack::push<f32>(f32 arg);
   template<> void LuaStack::push<f64>(f64 arg);
   template<> void LuaStack::push<LuaCustom>(LuaCustom arg);
   template<> void LuaStack::push<nullptr_t>(nullptr_t arg);

   template<> std::string LuaStack::to<std::string>(i32 stackIndex);
   template<> u32 LuaStack::to<u32>(i32 stackIndex);
   template<> i32 LuaStack::to<i32>(i32 stackIndex);
   template<> f32 LuaStack::to<f32>(i32 stackIndex);
   template<> f64 LuaStack::to<f64>(i32 stackIndex);
   template<> bool LuaStack::to<bool>(i32 stackIndex);

   template<> bool LuaStack::is<nullptr_t>(i32 stackIndex);
   template<> bool LuaStack::is<bool>(i32 stackIndex);
   template<> bool LuaStack::is<i32>(i32 stackIndex);
   template<> bool LuaStack::is<u32>(i32 stackIndex);
   template<> bool LuaStack::is<f32>(i32 stackIndex);
   template<> bool LuaStack::is<f64>(i32 stackIndex);
   template<> bool LuaStack::is<std::string>(i32 stackIndex);
   template<> bool LuaStack::is<LuaTable>(i32 stackIndex);
   template<> bool LuaStack::is<LuaFunction>(i32 stackIndex);


   template<typename T> T get(LuaStack& lua, const std::string& id, T valueIfMissing);
   template<typename T> T get(LuaStack& lua, i32 stackIndex, T valueIfMissing);

   template<> char get<char>(LuaStack& lua, const std::string& id, char valueIfMissing);
   template<> char get<char>(LuaStack& lua, i32 stackIndex, char valueIfMissing);
}

#include <lua/lua_stack_impl.h>
