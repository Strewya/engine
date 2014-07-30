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

	bool LuaStack::loadFile(const ResourceFile& file)
	{
		return (luaL_loadfile(m_L, file.getPath().c_str()) == LUA_OK);
	}

	bool LuaStack::doFile(const ResourceFile& file)
	{
		return (loadFile(file) && call());
	}

	uint32_t LuaStack::getTop()
	{
		return lua_gettop(m_L);
	}

	void LuaStack::pop(int32_t howMany)
	{
		lua_pop(m_L, howMany);
	}

	void LuaStack::pull(int32_t key)
	{
		int32_t index = getTop() == 0 ? LUA_GLOBALSINDEX : -2;
		lua_pushinteger(m_L, key);
		lua_gettable(m_L, index);
	}

	void LuaStack::pull(const std::string& key)
	{
		int32_t index = getTop() == 0 ? LUA_GLOBALSINDEX : -1;
		lua_getfield(m_L, index, key.c_str());
	}

	void LuaStack::pairs(int32_t index)
	{
		m_iters.emplace_back(Iteration{index < 0 ? getTop() + 1 + index : index, true});
		if( isTable(m_iters.back().m_iterateTableIndex) )
		{
			lua_pushnil(m_L);
		}
	}

	void LuaStack::pairs(const std::string& table)
	{
		pull(table);
		m_iters.emplace_back(Iteration{getTop(), true});
		if( isTable() )
		{
			lua_pushnil(m_L);
		}
	}

	void LuaStack::ipairs(int32_t index)
	{
		pairs(index);
		m_iters.back().m_iterateAll = false;
	}

	void LuaStack::ipairs(const std::string& table)
	{
		pairs(table);
		m_iters.back().m_iterateAll = false;
	}

	bool LuaStack::next()
	{
		int32_t res = 0;
		if( isTable(m_iters.back().m_iterateTableIndex) )
		{
			res = lua_next(m_L, m_iters.back().m_iterateTableIndex);
			if( res != 0 )
			{
				if( !m_iters.back().m_iterateAll && !isNumber(-2) )
				{
					do
					{
						pop();
						res = lua_next(m_L, m_iters.back().m_iterateTableIndex);
					} while( res != 0 && !isNumber(-2) );
				}
			}
		}
		if( res == 0 )
		{
			m_iters.pop_back();
		}
		return res != 0;
	}

	std::string LuaStack::toString(int32_t index)
	{
		return lua_tostring(m_L, index);
	}

	uint32_t LuaStack::toUint(int32_t index)
	{
		return lua_tointeger(m_L, index);
	}

	int32_t LuaStack::toInt(int32_t index)
	{
		return lua_tointeger(m_L, index);
	}

	float LuaStack::toFloat(int32_t index)
	{
		return (float)lua_tonumber(m_L, index);
	}

	double LuaStack::toDouble(int32_t index)
	{
		return (double)lua_tonumber(m_L, index);
	}

	bool LuaStack::toBool(int32_t index)
	{
		return lua_toboolean(m_L, index) == 1;
	}

	bool LuaStack::isNil(int32_t index)
	{
		return lua_isnil(m_L, index);
	}
	
	bool LuaStack::isString(int32_t index)
	{
		return (lua_isstring(m_L, index) == 1);
	}

	bool LuaStack::isFunction(int32_t index)
	{
		return lua_isfunction(m_L, index);
	}

	bool LuaStack::isTable(int32_t index)
	{
		return lua_istable(m_L, index);
	}

	bool LuaStack::isNumber(int32_t index)
	{
		return lua_isnumber(m_L, index) == 1;
	}

	bool LuaStack::isBool(int32_t index)
	{
		return lua_isboolean(m_L, index);
	}

	bool LuaStack::call()
	{
		return (lua_pcall(m_L, m_narg, (m_nres == 0 ? LUA_MULTRET : m_nres), 0) == LUA_OK);
	}
}
