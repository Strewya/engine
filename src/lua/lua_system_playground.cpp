//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "lua/lua_system.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "utility/utility.h"
/******* end headers *******/

namespace core
{
   void LuaStack::dumpStack()
   {
      u32 top = lua_gettop(m_L);
      for( u32 i = 1; i <= top; ++i )
      {  /* repeat for each level */
         int t = lua_type(m_L, i);
         std::cout << "[" << i << "]=";
         switch( t )
         {
            case LUA_TSTRING:  /* strings */
               std::cout << lua_tostring(m_L, i);
               break;

            case LUA_TBOOLEAN:  /* booleans */
               std::cout << lua_toboolean(m_L, i) ? "true" : "false";
               break;

            case LUA_TNUMBER:  /* numbers */
               std::cout << lua_tonumber(m_L, i);
               break;

            default:  /* other values */
               std::cout << lua_typename(m_L, t);
               break;

         }
         std::cout << ", ";
      }
      std::cout << std::endl;
   }

   void test_luaStack()
   {
      LuaSystem ls;
      ls.init();

      auto stack = ls.getStack();

      CORE_ASSERT(AssertLevel::Fatal, stack.getTop() == 0);

      auto loaded = stack.loadFile(CORE_RESOURCE("Defs/hedgehog.shee"));
      CORE_ASSERT(AssertLevel::Fatal, !loaded);
      CORE_ASSERT(AssertLevel::Fatal, stack.is<std::string>());
      CORE_LOG_DEBUG(stack.to<std::string>());
      stack.pop(1); //string error message
      CORE_ASSERT(AssertLevel::Fatal, stack.getTop() == 0);

      loaded = stack.loadFile(CORE_RESOURCE("Defs/hedgehog.sheet"));
      CORE_ASSERT(AssertLevel::Fatal, loaded);
      CORE_ASSERT(AssertLevel::Fatal, stack.is<LuaFunction>());

      auto called = stack.call();
      CORE_ASSERT(AssertLevel::Fatal, called);
      CORE_ASSERT(AssertLevel::Fatal, stack.is<LuaTable>());
      stack.pop(1); //table
      CORE_ASSERT(AssertLevel::Fatal, stack.getTop() == 0);

      //alternatively
      called = stack.doFile(CORE_RESOURCE("Defs/hedgehog.sheet")); //internally calls loadFile and call
      CORE_ASSERT(AssertLevel::Fatal, called);
      CORE_ASSERT(AssertLevel::Fatal, stack.is<LuaTable>());

      for( stack.pairs(); stack.next(); stack.pop(1)/*value*/ )
      {
         CORE_ASSERT(AssertLevel::Fatal, stack.is<std::string>(-2));
         CORE_ASSERT(AssertLevel::Fatal, stack.is<LuaTable>(-1));
         CORE_LOG_DEBUG("table name: ", stack.to<std::string>(-2));
      }

      for( stack.pairs("images"); stack.next(); stack.pop(1) )
      {
         CORE_ASSERT(AssertLevel::Fatal, stack.is<std::string>(-2));
         CORE_ASSERT(AssertLevel::Fatal, stack.is<LuaTable>());
         CORE_LOG_DEBUG("table name: ", stack.to<std::string>(-2));
         stack.pull("pos");
         CORE_ASSERT(AssertLevel::Fatal, stack.is<LuaTable>());
         stack.pull(1);
         CORE_ASSERT(AssertLevel::Fatal, stack.is<i32>() || stack.is<nullptr_t>());
         if( stack.is<nullptr_t>() )
         {
            stack.pop(1); //nil
            stack.pull("x");
            CORE_ASSERT(AssertLevel::Fatal, stack.is<i32>());
         }
         f32 x = stack.to<f32>();
         CORE_LOG_DEBUG("x: ", x);
         stack.pop(1); //number
         stack.pull(2);
         CORE_ASSERT(AssertLevel::Fatal, stack.is<i32>() || stack.is<nullptr_t>());
         if( stack.is<nullptr_t>() )
         {
            stack.pop(1); //nil
            stack.pull("y");
            CORE_ASSERT(AssertLevel::Fatal, stack.is<i32>());
         }
         f32 y = stack.to<f32>();
         CORE_LOG_DEBUG("y: ", y);
         stack.pop(1); //number
         stack.pull(3);
         CORE_ASSERT(AssertLevel::Fatal, stack.is<nullptr_t>());
         stack.pop(2); //nil and 'pos' table
         stack.pull("height");
         CORE_ASSERT(AssertLevel::Fatal, stack.is<nullptr_t>() || stack.is<u32>());
         if( stack.is<u32>() )
         {
            u32 height = stack.to<u32>();
            CORE_LOG_DEBUG("height: ", height);
         }
         stack.pop(1); //either nil or table
         stack.pull("width");
         CORE_ASSERT(AssertLevel::Fatal, stack.is<nullptr_t>() || stack.is<u32>());
         if( stack.is<u32>() )
         {
            u32 width = stack.to<u32>();
            CORE_LOG_DEBUG("width: ", width);
         }
         stack.pop(1); //either nil or table
      }
      CORE_ASSERT(AssertLevel::Fatal, stack.getTop() == 1);

      for( stack.pairs("animations"); stack.next(); stack.pop(1) )
      {
         CORE_ASSERT(AssertLevel::Fatal, stack.is<std::string>(-2));
         CORE_ASSERT(AssertLevel::Fatal, stack.is<LuaTable>(-1));
         CORE_LOG_DEBUG("table name: ", stack.to<std::string>(-2));
         stack.pull("loop");
         CORE_ASSERT(AssertLevel::Fatal, stack.is<nullptr_t>() || stack.is<bool>());
         if( stack.is<bool>() )
         {
            bool loops = stack.to<bool>();
            CORE_LOG_DEBUG("loops: ", loops);
         }
         stack.pop(1); // nil or bool
         stack.pull("duration");
         CORE_ASSERT(AssertLevel::Fatal, stack.is<nullptr_t>() || stack.is<f32>());
         if( stack.is<f32>() )
         {
            f32 duration = stack.to<f32>();
            CORE_LOG_DEBUG("duration: ", duration);
         }
         stack.pop(1); //nil or number
         stack.pull("images");
         CORE_ASSERT(AssertLevel::Fatal, stack.is<LuaTable>());
         for( stack.ipairs(); stack.next(); stack.pop(1) )
         {
            CORE_ASSERT(AssertLevel::Fatal, stack.is<u32>(-2));
            CORE_ASSERT(AssertLevel::Fatal, stack.is<std::string>(-1));
            auto img = stack.to<std::string>();
            CORE_LOG_DEBUG("image name: ", img);
         }
         stack.pop(1);
      }
      CORE_ASSERT(AssertLevel::Fatal, stack.getTop() == 1);

      stack.pop(1); //hedgehog.sheet

      called = stack.doFile(CORE_RESOURCE("Defs/font.font"));
      CORE_ASSERT(AssertLevel::Fatal, stack.is<LuaTable>());
      for( stack.pairs(); stack.next(); stack.pop(1) )
      {
         CORE_ASSERT(AssertLevel::Fatal, stack.is<std::string>(-2));
         CORE_ASSERT(AssertLevel::Fatal, stack.is<LuaTable>(-1));
         CORE_LOG_DEBUG("table name: ", stack.to<std::string>(-2));
         stack.pull("texture");
         CORE_ASSERT(AssertLevel::Fatal, stack.is<std::string>());
         auto tex = stack.to<std::string>();
         CORE_LOG_DEBUG("texture: ", tex);
         stack.pop(1); //texture string
         stack.pull("size");
         CORE_ASSERT(AssertLevel::Fatal, stack.is<u32>());
         u32 size = stack.to<u32>();
         CORE_LOG_DEBUG("size: ", size);
         stack.pop(1); //size number
         for( stack.ipairs("glyphs"); stack.next(); stack.pop(1) )
         {
            CORE_ASSERT(AssertLevel::Fatal, stack.is<u32>(-2));
            CORE_ASSERT(AssertLevel::Fatal, stack.is<LuaTable>(-1));
            stack.pull("char");
            CORE_ASSERT(AssertLevel::Fatal, stack.is<std::string>());
            char ascii = stack.to<std::string>().at(0);
            CORE_LOG_DEBUG("ascii: ", ascii);
            stack.pop(1); //char string
            stack.pull("left");
            CORE_ASSERT(AssertLevel::Fatal, stack.is<u32>());
            u32 left = stack.to<u32>();
            CORE_LOG_DEBUG("left: ", left);
            stack.pop(1); //left number
            stack.pull("right");
            CORE_ASSERT(AssertLevel::Fatal, stack.is<u32>());
            u32 right = stack.to<u32>();
            CORE_LOG_DEBUG("right: ", right);
            stack.pop(1); //right number
            stack.pull("top");
            CORE_ASSERT(AssertLevel::Fatal, stack.is<u32>());
            u32 top = stack.to<u32>();
            CORE_LOG_DEBUG("top: ", top);
            stack.pop(1); //top number
         }
      }
      stack.pop(1); //font.font table

      CORE_ASSERT(AssertLevel::Fatal, stack.getTop() == 0);

      called = stack.doFile(CORE_RESOURCE("Scripts/test.lua"));
      CORE_ASSERT(AssertLevel::Fatal, called);
      CORE_ASSERT(AssertLevel::Fatal, stack.getTop() == 0);

      stack.pull("test1", 0);
      CORE_ASSERT(AssertLevel::Fatal, stack.is<LuaFunction>());
      called = stack.call();
      CORE_ASSERT(AssertLevel::Fatal, called);
      CORE_ASSERT(AssertLevel::Fatal, stack.is<i32>());
      i32 n = stack.to<i32>();
      CORE_ASSERT(AssertLevel::Fatal, n == 1);
      stack.pop(1);
      CORE_ASSERT(AssertLevel::Fatal, stack.getTop() == 0);

      stack.pull("test2", 0);
      CORE_ASSERT(AssertLevel::Fatal, stack.is<LuaFunction>());
      called = stack.call(5);
      CORE_ASSERT(AssertLevel::Fatal, called);
      CORE_ASSERT(AssertLevel::Fatal, stack.is<i32>());
      n = stack.to<i32>();
      CORE_ASSERT(AssertLevel::Fatal, n == 5 + 2);
      stack.pop(1);
      CORE_ASSERT(AssertLevel::Fatal, stack.getTop() == 0);

      stack.pull("test3", 0);
      CORE_ASSERT(AssertLevel::Fatal, stack.is<LuaFunction>());
      called = stack.call(std::string("are you gay?"));
      CORE_ASSERT(AssertLevel::Fatal, called);
      CORE_ASSERT(AssertLevel::Fatal, stack.is<std::string>());
      auto s = stack.to<std::string>();
      CORE_ASSERT(AssertLevel::Fatal, s == "are you gay? yes");
      stack.pop(1);
      CORE_ASSERT(AssertLevel::Fatal, stack.getTop() == 0);

      stack.pull("test4", 0);
      CORE_ASSERT(AssertLevel::Fatal, stack.is<LuaFunction>());
      i32 a = 1; i32 b = 2; i32 c = 3;
      i32 x = 0; i32 y = 0; i32 z = 0;
      called = stack.call(a, b, c, &x, &y, &z);
      CORE_ASSERT(AssertLevel::Fatal, called);
      CORE_ASSERT(AssertLevel::Fatal, stack.getTop() == 0);
      CORE_ASSERT(AssertLevel::Fatal, a == x && b == y && c == z);
/*
      stack.pull("test5", 0);
      CORE_ASSERT(AssertLevel::Fatal, stack.is<LuaFunction>());
      Vec2 v{5, 5};
      called = stack.call(LuaCustom{&v, STR(Vec2)});
      CORE_ASSERT(AssertLevel::Fatal, called);
      CORE_ASSERT(AssertLevel::Fatal, stack.getTop() == 0);
      CORE_ASSERT(AssertLevel::Fatal, v.x == 1 && v.y == 1);
*/
      stack.pull("test3", 0);
      CORE_ASSERT(AssertLevel::Fatal, stack.is<LuaFunction>());
      called = stack.call(std::string("are you a monkey?"), &s);
      CORE_ASSERT(AssertLevel::Fatal, called);
      CORE_ASSERT(AssertLevel::Fatal, s == "are you a monkey? yes");
      CORE_ASSERT(AssertLevel::Fatal, stack.getTop() == 0);

      ls.shutdown();
   }
}