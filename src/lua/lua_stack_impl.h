#pragma once
/********************************************
*  contents:   #description
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
#include "utility/utility.h"
/******* end header inclusion *******/

namespace core
{
   template<typename T>
   void LuaStack::push(T arg)
   {
      CORE_ASSERT_ERR(false, "Invoked unspecialized LuaStack::push<", typeid(T).name(), ">()");
   }

   template<typename T>
   T LuaStack::to(i32 stackIndex)
   {
      CORE_ASSERT_ERR(false, "Invoked unspecialized LuaStack::to<", typeid(T).name(), ">()");
      return T{};
   }

   template<typename T>
   bool LuaStack::is(i32 stackIndex)
   {
      CORE_ASSERT_ERR(false, "Invoked unspecialized LuaStack::is<", typeid(T).name(), ">()");
      return false;
   }

   template<typename T, typename ...Args>
   bool LuaStack::call(T&& arg, Args... rest)
   {
      push(arg);
      ++m_narg;
      auto res = call(rest...);
      --m_narg;
      return res;
   }

   template<typename T, typename ...Args>
   bool LuaStack::call(T* arg, Args... rest)
   {
      ++m_nres;
      auto res = call(rest...);
      if( res )
      {
         if( is<T>() )
         {
            *arg = to<T>();
         }
         pop();
      }
      --m_nres;
      return res;
   }


   template<typename T> T get(LuaStack& lua, const char* id, T valueIfMissing)
   {
      lua.pull(id);
      if( lua.is<T>() )
      {
         valueIfMissing = lua.to<T>();
      }
      lua.pop();
      return valueIfMissing;
   }

   template<typename T> T get(LuaStack& lua, i32 stackIndex, T valueIfMissing)
   {
      lua.pull(stackIndex);
      if( lua.is<T>() )
      {
         valueIfMissing = lua.to<T>();
      }
      lua.pop();
      return valueIfMissing;
   }
}
