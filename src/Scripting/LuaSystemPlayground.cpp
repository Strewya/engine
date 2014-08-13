//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Scripting/LuaSystem.h>
/******* C++ headers *******/
#include <cassert>
/******* extra headers *******/
#include <Util/ResourceFile.h>
#include <Util/Utility.h>
#include <Util/Vec2.h>
/******* end headers *******/

namespace Core
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

	void runPlayground()
	{
		LuaSystem ls;
		ls.init();

		auto stack = ls.getStack();

		assert(stack.getTop() == 0);

		auto loaded = stack.loadFile("Defs/hedgehog.shee");
		assert(!loaded);
		assert(stack.isString());
		DEBUG_INFO(stack.toString());
		stack.pop(1); //string error message
		assert(stack.getTop() == 0);

		loaded = stack.loadFile("Defs/hedgehog.sheet");
		assert(loaded);
		assert(stack.isFunction());

		auto called = stack.call();
		assert(called);
		assert(stack.isTable());
		stack.pop(1); //table
		assert(stack.getTop() == 0);

		//alternatively
		called = stack.doFile("Defs/hedgehog.sheet"); //internally calls loadFile and call
		assert(called);
		assert(stack.isTable());

		for( stack.pairs(); stack.next(); stack.pop(1)/*value*/ )
		{
			assert(stack.isString(-2));
			assert(stack.isTable(-1));
			DEBUG_INFO("table name: ", stack.toString(-2));
		}

		for( stack.pairs("images"); stack.next(); stack.pop(1) )
		{
			assert(stack.isString(-2));
			assert(stack.isTable());
			DEBUG_INFO("table name: ", stack.toString(-2));
			stack.pull("pos");
			assert(stack.isTable());
			stack.pull(1);
			assert(stack.isNumber() || stack.isNil());
			if( stack.isNil() )
			{
				stack.pop(1); //nil
				stack.pull("x");
				assert(stack.isNumber());
			}
			float x = stack.toFloat();
			DEBUG_INFO("x: ", x);
			stack.pop(1); //number
			stack.pull(2);
			assert(stack.isNumber() || stack.isNil());
			if( stack.isNil() )
			{
				stack.pop(1); //nil
				stack.pull("y");
				assert(stack.isNumber());
			}
			float y = stack.toFloat();
			DEBUG_INFO("y: ", y);
			stack.pop(1); //number
			stack.pull(3);
			assert(stack.isNil());
			stack.pop(2); //nil and 'pos' table
			stack.pull("height");
			assert(stack.isNil() || stack.isNumber());
			if( stack.isNumber() )
			{
				uint32_t height = stack.toUint();
				DEBUG_INFO("height: ", height);
			}
			stack.pop(1); //either nil or table
			stack.pull("width");
			assert(stack.isNil() || stack.isNumber());
			if( stack.isNumber() )
			{
				uint32_t width = stack.toUint();
				DEBUG_INFO("width: ", width);
			}
			stack.pop(1); //either nil or table
		}
		assert(stack.getTop() == 1);

		for( stack.pairs("animations"); stack.next(); stack.pop(1) )
		{
			assert(stack.isString(-2));
			assert(stack.isTable(-1));
			DEBUG_INFO("table name: ", stack.toString(-2));
			stack.pull("loop");
			assert(stack.isNil() || stack.isBool());
			if( stack.isBool() )
			{
				bool loops = stack.toBool();
				DEBUG_INFO("loops: ", loops);
			}
			stack.pop(1); // nil or bool
			stack.pull("duration");
			assert(stack.isNil() || stack.isNumber());
			if( stack.isNumber() )
			{
				float duration = stack.toFloat();
				DEBUG_INFO("duration: ", duration);
			}
			stack.pop(1); //nil or number
			stack.pull("images");
			assert(stack.isTable());
			for( stack.ipairs(); stack.next(); stack.pop(1) )
			{
				assert(stack.isNumber(-2));
				assert(stack.isString(-1));
				auto img = stack.toString();
				DEBUG_INFO("image name: ", img);
			}
			stack.pop(1);
		}
		assert(stack.getTop() == 1);

		stack.pop(1); //hedgehog.sheet

		called = stack.doFile("Defs/font.font");
		assert(stack.isTable());
		for( stack.pairs(); stack.next(); stack.pop(1) )
		{
			assert(stack.isString(-2));
			assert(stack.isTable(-1));
			DEBUG_INFO("table name: ", stack.toString(-2));
			stack.pull("texture");
			assert(stack.isString());
			auto tex = stack.toString();
			DEBUG_INFO("texture: ", tex);
			stack.pop(1); //texture string
			stack.pull("size");
			assert(stack.isNumber());
			uint32_t size = stack.toUint();
			DEBUG_INFO("size: ", size);
			stack.pop(1); //size number
			for( stack.ipairs("glyphs"); stack.next(); stack.pop(1) )
			{
				assert(stack.isNumber(-2));
				assert(stack.isTable(-1));
				stack.pull("char");
				assert(stack.isString());
				char ascii = stack.toString().at(0);
				DEBUG_INFO("ascii: ", ascii);
				stack.pop(1); //char string
				stack.pull("left");
				assert(stack.isNumber());
				uint32_t left = stack.toUint();
				DEBUG_INFO("left: ", left);
				stack.pop(1); //left number
				stack.pull("right");
				assert(stack.isNumber());
				uint32_t right = stack.toUint();
				DEBUG_INFO("right: ", right);
				stack.pop(1); //right number
				stack.pull("top");
				assert(stack.isNumber());
				uint32_t top = stack.toUint();
				DEBUG_INFO("top: ", top);
				stack.pop(1); //top number
			}
		}
		stack.pop(1); //font.font table

		assert(stack.getTop() == 0);

		called = stack.doFile("Scripts/test.lua");
		assert(called);
		assert(stack.getTop() == 0);

		stack.pull("test1");
		assert(stack.isFunction());
		called = stack.call();
		assert(called);
		assert(stack.isNumber());
		int32_t n = stack.toInt();
		assert(n == 1);
		stack.pop(1);
		assert(stack.getTop() == 0);

		stack.pull("test2");
		assert(stack.isFunction());
		called = stack.call(5);
		assert(called);
		assert(stack.isNumber());
		n = stack.toInt();
		assert(n == 5 + 2);
		stack.pop(1);
		assert(stack.getTop() == 0);

		stack.pull("test3");
		assert(stack.isFunction());
		called = stack.call(std::string("are you gay?"));
		assert(called);
		assert(stack.isString());
		auto s = stack.toString();
		assert(s == "are you gay? yes");
		stack.pop(1);
		assert(stack.getTop() == 0);

		stack.pull("test4");
		assert(stack.isFunction());
		int32_t a = 1; int32_t b = 2; int32_t c = 3;
		int32_t x = 0; int32_t y = 0; int32_t z = 0;
		called = stack.call(a, b, c, &x, &y, &z);
		assert(called);
		assert(stack.getTop() == 0);
		assert(a == x && b == y && c == z);

		stack.pull("test5");
		assert(stack.isFunction());
		Vec2 v(5, 5);
		called = stack.call(CustomType{&v, CLASS(Vec2)});
		assert(called);
		assert(stack.getTop() == 0);
		assert(v.x == 1 && v.y == 1);

		stack.pull("test3");
		assert(stack.isFunction());
		called = stack.call(std::string("are you a monkey?"), &s);
		assert(called);
		assert(s == "are you a monkey? yes");
		assert(stack.getTop() == 0);

		ls.shutdown();
	}
}