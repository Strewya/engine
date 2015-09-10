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
      u32 n = lua_gettop(m_L);
      for( u32 i = 1; i <= n; ++i )
      {
         switch( lua_type(m_L, i) )
         {
            case LUA_TNUMBER:
            {
               CORE_LOG_DEBUG("[", i, "]=", lua_tonumber(m_L, i));
            } break;
            case LUA_TSTRING:
            {
               CORE_LOG_DEBUG("[", i, "]=", lua_tostring(m_L, i));
            } break;
            case LUA_TBOOLEAN:
            {
               CORE_LOG_DEBUG("[", i, "]=", lua_toboolean(m_L, i) == 1 ? "true" : "false");
            } break;
            default:
            {
               CORE_LOG_DEBUG("[", i, "]=", lua_typename(m_L, i), "(", lua_tonumber(m_L, i), ")");
            } break;
         }
      }
      CORE_LOG_DEBUG("-------------------");
   }

   void test_luaStack()
   {
      LuaSystem ls;
      //ls.init();

      auto stack = ls.getStack();

      CORE_ASSERT_FATAL(stack.getTop() == 0, "Playground error");

      auto loaded = stack.loadFile(CORE_RESOURCE("Defs/hedgehog.shee"));
      CORE_ASSERT_FATAL(!loaded, "Playground error");
      CORE_ASSERT_FATAL(stack.is<str>(), "Playground error");
      CORE_LOG_DEBUG(stack.to<str_writeable>());
      stack.pop(1); //string error message
      CORE_ASSERT_FATAL(stack.getTop() == 0, "Playground error");

      loaded = stack.loadFile(CORE_RESOURCE("Defs/hedgehog.sheet"));
      CORE_ASSERT_FATAL(loaded, "Playground error");
      CORE_ASSERT_FATAL(stack.is<LuaFunction>(), "Playground error");

      auto called = stack.call();
      CORE_ASSERT_FATAL(called, "Playground error");
      CORE_ASSERT_FATAL(stack.is<LuaTable>(), "Playground error");
      stack.pop(1); //table
      CORE_ASSERT_FATAL(stack.getTop() == 0, "Playground error");

      //alternatively
      called = stack.doFile(CORE_RESOURCE("Defs/hedgehog.sheet")); //internally calls loadFile and call
      CORE_ASSERT_FATAL(called, "Playground error");
      CORE_ASSERT_FATAL(stack.is<LuaTable>(), "Playground error");

      for( stack.pairs(); stack.next(); stack.pop(1)/*value*/ )
      {
         CORE_ASSERT_FATAL(stack.is<str>(-2), "Playground error");
         CORE_ASSERT_FATAL(stack.is<LuaTable>(-1), "Playground error");
         CORE_LOG_DEBUG("table name: ", stack.to<str_writeable>(-2));
      }

      for( stack.pairs("images"); stack.next(); stack.pop(1) )
      {
         CORE_ASSERT_FATAL(stack.is<str>(-2), "Playground error");
         CORE_ASSERT_FATAL(stack.is<LuaTable>(), "Playground error");
         CORE_LOG_DEBUG("table name: ", stack.to<str_writeable>(-2));
         stack.pull("pos");
         CORE_ASSERT_FATAL(stack.is<LuaTable>(), "Playground error");
         stack.pull(1);
         CORE_ASSERT_FATAL(stack.is<i32>() || stack.is<nullptr_t>(), "Playground error");
         if( stack.is<nullptr_t>() )
         {
            stack.pop(1); //nil
            stack.pull("x");
            CORE_ASSERT_FATAL(stack.is<i32>(), "Playground error");
         }
         f32 x = stack.to<f32>();
         CORE_LOG_DEBUG("x: ", x);
         stack.pop(1); //number
         stack.pull(2);
         CORE_ASSERT_FATAL(stack.is<i32>() || stack.is<nullptr_t>(), "Playground error");
         if( stack.is<nullptr_t>() )
         {
            stack.pop(1); //nil
            stack.pull("y");
            CORE_ASSERT_FATAL(stack.is<i32>(), "Playground error");
         }
         f32 y = stack.to<f32>();
         CORE_LOG_DEBUG("y: ", y);
         stack.pop(1); //number
         stack.pull(3);
         CORE_ASSERT_FATAL(stack.is<nullptr_t>(), "Playground error");
         stack.pop(2); //nil and 'pos' table
         stack.pull("height");
         CORE_ASSERT_FATAL(stack.is<nullptr_t>() || stack.is<u32>(), "Playground error");
         if( stack.is<u32>() )
         {
            u32 height = stack.to<u32>();
            CORE_LOG_DEBUG("height: ", height);
         }
         stack.pop(1); //either nil or table
         stack.pull("width");
         CORE_ASSERT_FATAL(stack.is<nullptr_t>() || stack.is<u32>(), "Playground error");
         if( stack.is<u32>() )
         {
            u32 width = stack.to<u32>();
            CORE_LOG_DEBUG("width: ", width);
         }
         stack.pop(1); //either nil or table
      }
      CORE_ASSERT_FATAL(stack.getTop() == 1, "Playground error");

      for( stack.pairs("animations"); stack.next(); stack.pop(1) )
      {
         CORE_ASSERT_FATAL(stack.is<str>(-2), "Playground error");
         CORE_ASSERT_FATAL(stack.is<LuaTable>(-1), "Playground error");
         CORE_LOG_DEBUG("table name: ", stack.to<str_writeable>(-2));
         stack.pull("loop");
         CORE_ASSERT_FATAL(stack.is<nullptr_t>() || stack.is<bool>(), "Playground error");
         if( stack.is<bool>() )
         {
            bool loops = stack.to<bool>();
            CORE_LOG_DEBUG("loops: ", loops);
         }
         stack.pop(1); // nil or bool
         stack.pull("duration");
         CORE_ASSERT_FATAL(stack.is<nullptr_t>() || stack.is<f32>(), "Playground error");
         if( stack.is<f32>() )
         {
            f32 duration = stack.to<f32>();
            CORE_LOG_DEBUG("duration: ", duration);
         }
         stack.pop(1); //nil or number
         stack.pull("images");
         CORE_ASSERT_FATAL(stack.is<LuaTable>(), "Playground error");
         for( stack.ipairs(); stack.next(); stack.pop(1) )
         {
            CORE_ASSERT_FATAL(stack.is<u32>(-2), "Playground error");
            CORE_ASSERT_FATAL(stack.is<str>(-1), "Playground error");
            auto img = stack.to<str_writeable>();
            CORE_LOG_DEBUG("image name: ", img);
         }
         stack.pop(1);
      }
      CORE_ASSERT_FATAL(stack.getTop() == 1, "Playground error");

      stack.pop(1); //hedgehog.sheet

      called = stack.doFile(CORE_RESOURCE("Defs/font.font"));
      CORE_ASSERT_FATAL(stack.is<LuaTable>(), "Playground error");
      for( stack.pairs(); stack.next(); stack.pop(1) )
      {
         CORE_ASSERT_FATAL(stack.is<str>(-2), "Playground error");
         CORE_ASSERT_FATAL(stack.is<LuaTable>(-1), "Playground error");
         CORE_LOG_DEBUG("table name: ", stack.to<str_writeable>(-2));
         stack.pull("texture");
         CORE_ASSERT_FATAL(stack.is<str>(), "Playground error");
         auto tex = stack.to<str_writeable>();
         CORE_LOG_DEBUG("texture: ", tex);
         stack.pop(1); //texture string
         stack.pull("size");
         CORE_ASSERT_FATAL(stack.is<u32>(), "Playground error");
         u32 size = stack.to<u32>();
         CORE_LOG_DEBUG("size: ", size);
         stack.pop(1); //size number
         for( stack.ipairs("glyphs"); stack.next(); stack.pop(1) )
         {
            CORE_ASSERT_FATAL(stack.is<u32>(-2), "Playground error");
            CORE_ASSERT_FATAL(stack.is<LuaTable>(-1), "Playground error");
            stack.pull("char");
            CORE_ASSERT_FATAL(stack.is<str>(), "Playground error");
            char ascii = stack.to<str_writeable>().buffer[0];
            CORE_LOG_DEBUG("ascii: ", ascii);
            stack.pop(1); //char string
            stack.pull("left");
            CORE_ASSERT_FATAL(stack.is<u32>(), "Playground error");
            u32 left = stack.to<u32>();
            CORE_LOG_DEBUG("left: ", left);
            stack.pop(1); //left number
            stack.pull("right");
            CORE_ASSERT_FATAL(stack.is<u32>(), "Playground error");
            u32 right = stack.to<u32>();
            CORE_LOG_DEBUG("right: ", right);
            stack.pop(1); //right number
            stack.pull("top");
            CORE_ASSERT_FATAL(stack.is<u32>(), "Playground error");
            u32 top = stack.to<u32>();
            CORE_LOG_DEBUG("top: ", top);
            stack.pop(1); //top number
         }
      }
      stack.pop(1); //font.font table

      CORE_ASSERT_FATAL(stack.getTop() == 0, "Playground error");

      called = stack.doFile(CORE_RESOURCE("Scripts/test.lua"));
      CORE_ASSERT_FATAL(called, "Playground error");
      CORE_ASSERT_FATAL(stack.getTop() == 0, "Playground error");

      stack.pull("test1", 0);
      CORE_ASSERT_FATAL(stack.is<LuaFunction>(), "Playground error");
      called = stack.call();
      CORE_ASSERT_FATAL(called, "Playground error");
      CORE_ASSERT_FATAL(stack.is<i32>(), "Playground error");
      i32 n = stack.to<i32>();
      CORE_ASSERT_FATAL(n == 1, "Playground error");
      stack.pop(1);
      CORE_ASSERT_FATAL(stack.getTop() == 0, "Playground error");

      stack.pull("test2", 0);
      CORE_ASSERT_FATAL(stack.is<LuaFunction>(), "Playground error");
      called = stack.call(5);
      CORE_ASSERT_FATAL(called, "Playground error");
      CORE_ASSERT_FATAL(stack.is<i32>(), "Playground error");
      n = stack.to<i32>();
      CORE_ASSERT_FATAL(n == 5 + 2, "Playground error");
      stack.pop(1);
      CORE_ASSERT_FATAL(stack.getTop() == 0, "Playground error");

      stack.pull("test3", 0);
      CORE_ASSERT_FATAL(stack.is<LuaFunction>(), "Playground error");
      called = stack.call(str{"are you gay?"});
      CORE_ASSERT_FATAL(called, "Playground error");
      CORE_ASSERT_FATAL(stack.is<str>(), "Playground error");
      auto s = stack.to<str_writeable>();
      CORE_ASSERT_FATAL(s == "are you gay? yes", "Playground error");
      stack.pop(1);
      CORE_ASSERT_FATAL(stack.getTop() == 0, "Playground error");

      stack.pull("test4", 0);
      CORE_ASSERT_FATAL(stack.is<LuaFunction>(), "Playground error");
      i32 a = 1; i32 b = 2; i32 c = 3;
      i32 x = 0; i32 y = 0; i32 z = 0;
      called = stack.call(a, b, c, &x, &y, &z);
      CORE_ASSERT_FATAL(called, "Playground error");
      CORE_ASSERT_FATAL(stack.getTop() == 0, "Playground error");
      CORE_ASSERT_FATAL(a == x && b == y && c == z, "Playground error");
      /*
            stack.pull("test5", 0);
            CORE_ASSERT_FATAL(stack.is<LuaFunction>(), "Playground error");
            Vec2 v{5, 5};
            called = stack.call(LuaCustom{&v, STR(Vec2)});
            CORE_ASSERT_FATAL(called, "Playground error");
            CORE_ASSERT_FATAL(stack.getTop() == 0, "Playground error");
            CORE_ASSERT_FATAL(v.x == 1 && v.y == 1, "Playground error");
            */
      stack.pull("test3", 0);
      CORE_ASSERT_FATAL(stack.is<LuaFunction>(), "Playground error");
      called = stack.call(str{"are you a monkey?"}, &s);
      CORE_ASSERT_FATAL(called, "Playground error");
      CORE_ASSERT_FATAL(s == "are you a monkey? yes", "Playground error");
      CORE_ASSERT_FATAL(stack.getTop() == 0, "Playground error");

      ls.shutdown();
   }
}