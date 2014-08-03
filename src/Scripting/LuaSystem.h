#pragma once
/********************************************
*	class:	LuaSystem
*	usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
/******* common headers *******/
#include <Scripting/LuaInclude.h>
/******* extra headers *******/
#include <Scripting/LuaStack.h>
/******* end header inclusion *******/

namespace Core
{
	class LuaSystem
	{
	public:
		bool init();
		bool shutdown();
		void update();

		LuaStack getStack() const;


	private:
		lua_State* m_L;
	};

	void runPlayground();
}
