//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Scripting/LuaSystem.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Scripting/luaBinding.h>
#include <Util/ResourceFile.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	bool LuaSystem::init()
	{
		bool status = false;
		m_L = luaL_newstate();
		if( m_L )
		{
			luaL_openlibs(m_L);
			tolua_core_open(m_L);
			status = true;
		}
		DEBUG_INIT(LuaSystem);
		return status;
	}

	bool LuaSystem::shutdown()
	{
		bool status = true;
		lua_close(m_L);
		DEBUG_SHUTDOWN(LuaSystem);
		return status;
	}

	void LuaSystem::update()
	{
		lua_gc(m_L, LUA_GCSTEP, 20);
	}

	LuaStack LuaSystem::getStack() const
	{
		return LuaStack(m_L);
	}




	LuaStack::LuaStack(lua_State* L)
		: m_L(L), m_narg(0), m_nres(0)
	{}

	bool LuaStack::loadFile(const ResourceFile& file) const
	{
		return (luaL_loadfile(m_L, file.getPath().c_str()) == LUA_OK);
	}

	uint32_t LuaStack::getTop() const
	{
		return lua_gettop(m_L);
	}

	std::string LuaStack::toString(int32_t index) const
	{
		return lua_tostring(m_L, index);
	}

	bool LuaStack::isString(int32_t index) const
	{
		return (lua_isstring(m_L, index) == 1);
	}

	bool LuaStack::isFunction(int32_t index) const
	{
		return (lua_isfunction(m_L, index) == 1);
	}

	bool LuaStack::isTable(int32_t index) const
	{
		return (lua_istable(m_L, index) == 1);
	}

	void LuaStack::pop(int32_t howMany) const
	{
		lua_pop(m_L, howMany);
	}

	bool LuaStack::operator()()
	{
		return (lua_pcall(m_L, m_narg, (m_nres == 0 ? LUA_MULTRET : m_nres), 0) == LUA_OK);
	}

	bool LuaStack::operator()(const int32_t& arg)
	{
		lua_pushinteger(m_L, arg);
		++m_narg;
		return operator()();
	}

	bool LuaStack::operator()(int32_t* arg)
	{
		++m_nres;
		auto res = operator()();
		if( res && arg != nullptr )
		{
			*arg = lua_tointeger(m_L, -1);
			pop();
		}
		return res;
	}
}
