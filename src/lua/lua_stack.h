#pragma once
/********************************************
*  contents:   #description
*  usage:
********************************************/
/******* c++ headers *******/
#include <cstdint>
#include <string>
#include <vector>
/******* common headers *******/
#include "lua/lua_include.h"
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

      uint32_t getTop();
      void pop(int32_t howMany = 1);
      void pull(uint32_t key, int32_t stackIndex = -1);
      void pull(const std::string& key, int32_t stackIndex = -1);

      void pairs(int32_t stackIndex = -1);
      void pairs(const std::string& table);
      void ipairs(int32_t stackIndex = -1);
      void ipairs(const std::string& table);

      bool next();

      void setValue(const std::string& key, int32_t stackIndex = -1);

      template<typename T> void push(T arg);

      template<typename T> T to(int32_t stackIndex = -1);

      template<typename T> bool is(int32_t stackIndex = -1);

      bool call();
      template<typename T, typename ...Args> bool call(T&& arg, Args... rest);
      template<typename T, typename ...Args> bool call(T* arg, Args... rest);

   private:
      lua_State* m_L;
      uint32_t m_narg;
      uint32_t m_nres;
      struct Iteration
      {
         int32_t m_iterateTableIndex;
         bool m_iterateAll;
         bool m_autoPopTable;
      };
      std::vector<Iteration> m_iters;
   };

   template<> void LuaStack::push<std::string>(std::string arg);
   template<> void LuaStack::push<int32_t>(int32_t arg);
   template<> void LuaStack::push<uint32_t>(uint32_t arg);
   template<> void LuaStack::push<bool>(bool arg);
   template<> void LuaStack::push<float>(float arg);
   template<> void LuaStack::push<double>(double arg);
   template<> void LuaStack::push<LuaCustom>(LuaCustom arg);
   template<> void LuaStack::push<nullptr_t>(nullptr_t arg);

   template<> std::string LuaStack::to<std::string>(int32_t stackIndex);
   template<> uint32_t LuaStack::to<uint32_t>(int32_t stackIndex);
   template<> int32_t LuaStack::to<int32_t>(int32_t stackIndex);
   template<> float LuaStack::to<float>(int32_t stackIndex);
   template<> double LuaStack::to<double>(int32_t stackIndex);
   template<> bool LuaStack::to<bool>(int32_t stackIndex);

   template<> bool LuaStack::is<nullptr_t>(int32_t stackIndex);
   template<> bool LuaStack::is<bool>(int32_t stackIndex);
   template<> bool LuaStack::is<int32_t>(int32_t stackIndex);
   template<> bool LuaStack::is<uint32_t>(int32_t stackIndex);
   template<> bool LuaStack::is<float>(int32_t stackIndex);
   template<> bool LuaStack::is<double>(int32_t stackIndex);
   template<> bool LuaStack::is<std::string>(int32_t stackIndex);
   template<> bool LuaStack::is<LuaTable>(int32_t stackIndex);
   template<> bool LuaStack::is<LuaFunction>(int32_t stackIndex);


   template<typename T> T get(LuaStack& lua, const std::string& id, T valueIfMissing);
   template<typename T> T get(LuaStack& lua, int32_t stackIndex, T valueIfMissing);

   template<> char get<char>(LuaStack& lua, const std::string& id, char valueIfMissing);
   template<> char get<char>(LuaStack& lua, int32_t stackIndex, char valueIfMissing);
}

#include <lua/lua_stack_impl.h>
