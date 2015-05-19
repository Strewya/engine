//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "lua/lua_system.h"
/******* c++ headers *******/
#include <cassert>
/******* extra headers *******/
#include "util/utility.h"
#include "util/geometry/vec2.h"
/******* end headers *******/

namespace core
{
   void LuaStack::dumpStack()
   {
      uint32_t top = lua_gettop(m_L);
      for( uint32_t i = 1; i <= top; ++i )
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

      assert(stack.getTop() == 0);

      auto loaded = stack.loadFile(CORE_RESOURCE("Defs/hedgehog.shee"));
      assert(!loaded);
      assert(stack.is<std::string>());
      CORE_INFO(stack.to<std::string>());
      stack.pop(1); //string error message
      assert(stack.getTop() == 0);

      loaded = stack.loadFile(CORE_RESOURCE("Defs/hedgehog.sheet"));
      assert(loaded);
      assert(stack.is<LuaFunction>());

      auto called = stack.call();
      assert(called);
      assert(stack.is<LuaTable>());
      stack.pop(1); //table
      assert(stack.getTop() == 0);

      //alternatively
      called = stack.doFile(CORE_RESOURCE("Defs/hedgehog.sheet")); //internally calls loadFile and call
      assert(called);
      assert(stack.is<LuaTable>());

      for( stack.pairs(); stack.next(); stack.pop(1)/*value*/ )
      {
         assert(stack.is<std::string>(-2));
         assert(stack.is<LuaTable>(-1));
         CORE_INFO("table name: ", stack.to<std::string>(-2));
      }

      for( stack.pairs("images"); stack.next(); stack.pop(1) )
      {
         assert(stack.is<std::string>(-2));
         assert(stack.is<LuaTable>());
         CORE_INFO("table name: ", stack.to<std::string>(-2));
         stack.pull("pos");
         assert(stack.is<LuaTable>());
         stack.pull(1);
         assert(stack.is<int32_t>() || stack.is<nullptr_t>());
         if( stack.is<nullptr_t>() )
         {
            stack.pop(1); //nil
            stack.pull("x");
            assert(stack.is<int32_t>());
         }
         float x = stack.to<float>();
         CORE_INFO("x: ", x);
         stack.pop(1); //number
         stack.pull(2);
         assert(stack.is<int32_t>() || stack.is<nullptr_t>());
         if( stack.is<nullptr_t>() )
         {
            stack.pop(1); //nil
            stack.pull("y");
            assert(stack.is<int32_t>());
         }
         float y = stack.to<float>();
         CORE_INFO("y: ", y);
         stack.pop(1); //number
         stack.pull(3);
         assert(stack.is<nullptr_t>());
         stack.pop(2); //nil and 'pos' table
         stack.pull("height");
         assert(stack.is<nullptr_t>() || stack.is<uint32_t>());
         if( stack.is<uint32_t>() )
         {
            uint32_t height = stack.to<uint32_t>();
            CORE_INFO("height: ", height);
         }
         stack.pop(1); //either nil or table
         stack.pull("width");
         assert(stack.is<nullptr_t>() || stack.is<uint32_t>());
         if( stack.is<uint32_t>() )
         {
            uint32_t width = stack.to<uint32_t>();
            CORE_INFO("width: ", width);
         }
         stack.pop(1); //either nil or table
      }
      assert(stack.getTop() == 1);

      for( stack.pairs("animations"); stack.next(); stack.pop(1) )
      {
         assert(stack.is<std::string>(-2));
         assert(stack.is<LuaTable>(-1));
         CORE_INFO("table name: ", stack.to<std::string>(-2));
         stack.pull("loop");
         assert(stack.is<nullptr_t>() || stack.is<bool>());
         if( stack.is<bool>() )
         {
            bool loops = stack.to<bool>();
            CORE_INFO("loops: ", loops);
         }
         stack.pop(1); // nil or bool
         stack.pull("duration");
         assert(stack.is<nullptr_t>() || stack.is<float>());
         if( stack.is<float>() )
         {
            float duration = stack.to<float>();
            CORE_INFO("duration: ", duration);
         }
         stack.pop(1); //nil or number
         stack.pull("images");
         assert(stack.is<LuaTable>());
         for( stack.ipairs(); stack.next(); stack.pop(1) )
         {
            assert(stack.is<uint32_t>(-2));
            assert(stack.is<std::string>(-1));
            auto img = stack.to<std::string>();
            CORE_INFO("image name: ", img);
         }
         stack.pop(1);
      }
      assert(stack.getTop() == 1);

      stack.pop(1); //hedgehog.sheet

      called = stack.doFile(CORE_RESOURCE("Defs/font.font"));
      assert(stack.is<LuaTable>());
      for( stack.pairs(); stack.next(); stack.pop(1) )
      {
         assert(stack.is<std::string>(-2));
         assert(stack.is<LuaTable>(-1));
         CORE_INFO("table name: ", stack.to<std::string>(-2));
         stack.pull("texture");
         assert(stack.is<std::string>());
         auto tex = stack.to<std::string>();
         CORE_INFO("texture: ", tex);
         stack.pop(1); //texture string
         stack.pull("size");
         assert(stack.is<uint32_t>());
         uint32_t size = stack.to<uint32_t>();
         CORE_INFO("size: ", size);
         stack.pop(1); //size number
         for( stack.ipairs("glyphs"); stack.next(); stack.pop(1) )
         {
            assert(stack.is<uint32_t>(-2));
            assert(stack.is<LuaTable>(-1));
            stack.pull("char");
            assert(stack.is<std::string>());
            char ascii = stack.to<std::string>().at(0);
            CORE_INFO("ascii: ", ascii);
            stack.pop(1); //char string
            stack.pull("left");
            assert(stack.is<uint32_t>());
            uint32_t left = stack.to<uint32_t>();
            CORE_INFO("left: ", left);
            stack.pop(1); //left number
            stack.pull("right");
            assert(stack.is<uint32_t>());
            uint32_t right = stack.to<uint32_t>();
            CORE_INFO("right: ", right);
            stack.pop(1); //right number
            stack.pull("top");
            assert(stack.is<uint32_t>());
            uint32_t top = stack.to<uint32_t>();
            CORE_INFO("top: ", top);
            stack.pop(1); //top number
         }
      }
      stack.pop(1); //font.font table

      assert(stack.getTop() == 0);

      called = stack.doFile(CORE_RESOURCE("Scripts/test.lua"));
      assert(called);
      assert(stack.getTop() == 0);

      stack.pull("test1", 0);
      assert(stack.is<LuaFunction>());
      called = stack.call();
      assert(called);
      assert(stack.is<int32_t>());
      int32_t n = stack.to<int32_t>();
      assert(n == 1);
      stack.pop(1);
      assert(stack.getTop() == 0);

      stack.pull("test2", 0);
      assert(stack.is<LuaFunction>());
      called = stack.call(5);
      assert(called);
      assert(stack.is<int32_t>());
      n = stack.to<int32_t>();
      assert(n == 5 + 2);
      stack.pop(1);
      assert(stack.getTop() == 0);

      stack.pull("test3", 0);
      assert(stack.is<LuaFunction>());
      called = stack.call(std::string("are you gay?"));
      assert(called);
      assert(stack.is<std::string>());
      auto s = stack.to<std::string>();
      assert(s == "are you gay? yes");
      stack.pop(1);
      assert(stack.getTop() == 0);

      stack.pull("test4", 0);
      assert(stack.is<LuaFunction>());
      int32_t a = 1; int32_t b = 2; int32_t c = 3;
      int32_t x = 0; int32_t y = 0; int32_t z = 0;
      called = stack.call(a, b, c, &x, &y, &z);
      assert(called);
      assert(stack.getTop() == 0);
      assert(a == x && b == y && c == z);
/*
      stack.pull("test5", 0);
      assert(stack.is<LuaFunction>());
      vec2f v{5, 5};
      called = stack.call(LuaCustom{&v, STR(vec2f)});
      assert(called);
      assert(stack.getTop() == 0);
      assert(v.x == 1 && v.y == 1);
*/
      stack.pull("test3", 0);
      assert(stack.is<LuaFunction>());
      called = stack.call(std::string("are you a monkey?"), &s);
      assert(called);
      assert(s == "are you a monkey? yes");
      assert(stack.getTop() == 0);

      ls.shutdown();
   }
}