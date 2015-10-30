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

      //loads a file as a function onto the stack without executing it
      bool loadFile(const char* file);
      //loads a file as a function and executes it
      bool doFile(const char* file);
      //get number of elements on the stack
      u32 getTop();
      //pops n elements (default: 1) off the stack
      void pop(i32 howMany = 1);
      //pulls onto the stack the value at array index from the table at stack index (default: top)
      void pull(u32 arrayIndex, i32 tableIndexOnStack = -1);
      //pulls onto the stack the value bound to 'key' from the table at stack index (default: top)
      void pull(const char* key, i32 tableIndexOnStack = -1);
      //iterates over table at stack index (default: top) via indices
      void pairs(i32 stackIndex = -1);
      //iterates over named table via indices
      void pairs(const char* table);
      //iterates over table at stack index (default: top) via strings and indices in undetermined order
      void ipairs(i32 stackIndex = -1);
      //iterates over named table via strings and indices in undetermined order
      void ipairs(const char* table);
      //used in combination with pairs/ipairs, returns true if there are more elements
      bool next();
      //sets the last pushed value to the table at stack index (default: top) and binds it to key
      void setValue(const char* key, i32 stackIndex = -2);
      
      //pushes the argument onto the stack
      template<typename T> void push(T arg);
      //casts the value at top of the stack to template argument
      template<typename T> T to(i32 stackIndex = -1);
      //tests whether the value at the top of the stack is of certain type
      template<typename T> bool is(i32 stackIndex = -1);
      
      //calls a function
      bool call();
      //calls a function with the specified arguments
      template<typename T, typename ...Args> bool call(T&& arg, Args... rest);
      //calls a function with the specified arguments
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
      enum
      {
         MaxIterations = 20,
      };
      Iteration m_iters[MaxIterations];
      u32 m_freeIterationSlot;
   };

   template<> void LuaStack::push<str>(str arg);
   template<> void LuaStack::push<i32>(i32 arg);
   template<> void LuaStack::push<u32>(u32 arg);
   template<> void LuaStack::push<bool>(bool arg);
   template<> void LuaStack::push<f32>(f32 arg);
   template<> void LuaStack::push<f64>(f64 arg);
   template<> void LuaStack::push<LuaCustom>(LuaCustom arg);
   template<> void LuaStack::push<nullptr_t>(nullptr_t arg);

   template<> str LuaStack::to<str>(i32 stackIndex);
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
   template<> bool LuaStack::is<str>(i32 stackIndex);
   template<> bool LuaStack::is<LuaTable>(i32 stackIndex);
   template<> bool LuaStack::is<LuaFunction>(i32 stackIndex);


   template<typename T> T get(LuaStack& lua, const char* id, T valueIfMissing);
   template<typename T> T get(LuaStack& lua, i32 stackIndex, T valueIfMissing);

   template<> char get<char>(LuaStack& lua, const char* id, char valueIfMissing);
   template<> char get<char>(LuaStack& lua, i32 stackIndex, char valueIfMissing);




}

#include <lua/lua_stack_impl.h>
