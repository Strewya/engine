//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
//none, because this is a header file with template code
/******* personal header *******/
#include <Scripting/LuaStack.h>
/******* C++ headers *******/
#include <cstdint>
#include <string>
/******* extra headers *******/
/******* end headers *******/

namespace Core
{
	template<typename T>
	void LuaStack::push(T arg)
	{
		throw std::logic_error(std::string("Invoked unspecialized LuaStack::push<") + typeid(T).name() + ">()");
	}

	template<typename T>
	T LuaStack::to(int32_t stackIndex)
	{
		throw std::logic_error(std::string("Invoked unspecialized LuaStack::to<") + typeid(T).name() + ">()");
	}

	template<typename T>
	bool LuaStack::is(int32_t stackIndex)
	{
		throw std::logic_error(std::string("Invoked unspecialized LuaStack::is<") + typeid(T).name() + ">()");
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


	template<typename T> T get(LuaStack& lua, const std::string& id, T valueIfMissing)
	{
		lua.pull(id);
		if( lua.is<T>() )
		{
			valueIfMissing = lua.to<T>();
		}
		lua.pop();
		return valueIfMissing;
	}

	template<typename T> T get(LuaStack& lua, int32_t stackIndex, T valueIfMissing)
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
