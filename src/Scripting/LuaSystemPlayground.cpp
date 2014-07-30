//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Scripting/LuaSystem.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Util/ResourceFile.h>
#include <Util/Utility.h>
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

		auto top = stack.getTop();
		assert(top == 0);

		auto loaded = stack.loadFile("Defs/hedgehog.shee");
		assert(loaded == false);
		assert(stack.isString());
		DEBUG_INFO(stack.toString());
		stack.pop();

		loaded = stack.loadFile("Defs/hedgehog.sheet");
		assert(loaded == true);
		assert(stack.isFunction());

		auto called = stack();
		assert(called == true);
		assert(stack.isTable());

		



		ls.shutdown();
	}
}