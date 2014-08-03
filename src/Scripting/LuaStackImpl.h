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
	template<typename ...Args>
	bool LuaStack::call(int32_t arg, Args... rest)
	{
		lua_pushinteger(m_L, arg);
		++m_narg;
		auto res = call(rest...);
		--m_narg;
		return res;
	}

	template<typename ...Args>
	bool LuaStack::call(uint32_t arg, Args... rest)
	{
		lua_pushinteger(m_L, arg);
		++m_narg;
		auto res = call(rest...);
		--m_narg;
		return res;
	}

	template<typename ...Args>
	bool LuaStack::call(bool arg, Args... rest)
	{
		lua_pushboolean(m_L, arg);
		++m_narg;
		auto res = call(rest...);
		--m_narg;
		return res;
	}

	template<typename ...Args>
	bool LuaStack::call(float arg, Args... rest)
	{
		lua_pushnumber(m_L, arg);
		++m_narg;
		auto res = call(rest...);
		--m_narg;
		return res;
	}

	template<typename ...Args>
	bool LuaStack::call(double arg, Args... rest)
	{
		lua_pushnumber(m_L, arg);
		++m_narg;
		auto res = call(rest...);
		--m_narg;
		return res;
	}

	template<typename ...Args>
	bool LuaStack::call(char arg, Args... rest)
	{
		lua_pushstring(m_L, arg);
		++m_narg;
		auto res = call(rest...);
		--m_narg;
		return res;
	}

	template<typename ...Args>
	bool LuaStack::call(const std::string& arg, Args... rest)
	{
		lua_pushstring(m_L, arg.c_str());
		++m_narg;
		auto res = call(rest...);
		--m_narg;
		return res;
	}

	template<typename ...Args>
	bool LuaStack::call(const CustomType& arg, Args... rest)
	{
		tolua_pushusertype(m_L, arg.ptr, arg.name);
		++m_narg;
		auto res = call(rest...);
		--m_narg;
		return res;
	}

	template<typename ...Args>
	bool LuaStack::call(int32_t* arg, Args... rest)
	{
		++m_nres;
		auto res = call(rest...);
		if( res )
		{
			if( isNumber() )
			{
				*arg = toInt();
			}
			pop();
		}
		--m_nres;
		return res;
	}

	template<typename ...Args>
	bool LuaStack::call(uint32_t* arg, Args... rest)
	{
		++m_nres;
		auto res = call(rest...);
		if( res )
		{
			if( isNumber() )
			{
				*arg = toUint();
			}
			pop();
		}
		--m_nres;
		return res;
	}

	template<typename ...Args>
	bool LuaStack::call(bool* arg, Args... rest)
	{
		++m_nres;
		auto res = call(rest...);
		if( res )
		{
			if( isBool() )
			{
				*arg = toBool();
			}
			pop();
		}
		--m_nres;
		return res;
	}

	template<typename ...Args>
	bool LuaStack::call(float* arg, Args... rest)
	{
		++m_nres;
		auto res = call(rest...);
		if( res )
		{
			if( isNumber() )
			{
				*arg = toFloat();
			}
			pop();
		}
		--m_nres;
		return res;
	}

	template<typename ...Args>
	bool LuaStack::call(double* arg, Args... rest)
	{
		++m_nres;
		auto res = call(rest...);
		if( res )
		{
			if( isNumber() )
			{
				*arg = toDouble();
			}
			pop();
		}
		--m_nres;
		return res;
	}

	template<typename ...Args>
	bool LuaStack::call(std::string* arg, Args... rest)
	{
		++m_nres;
		auto res = call(rest...);
		if( res )
		{
			if( isString() )
			{
				*arg = toString();
			}
			pop();
		}
		--m_nres;
		return res;
	}
}
