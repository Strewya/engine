//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Scripting/LuaStack.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Util/ResourceFile.h>
/******* end headers *******/

namespace Core
{
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
		m_iters.emplace_back(Iteration{index < 0 ? getTop() + 1 + index : index, true, false});
		if( isTable(m_iters.back().m_iterateTableIndex) )
		{
			lua_pushnil(m_L);
		}
	}

	void LuaStack::pairs(const std::string& table)
	{
		pull(table);
		m_iters.emplace_back(Iteration{getTop(), true, true});
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
			if( m_iters.back().m_autoPopTable )
			{
				pop();
			}
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
		auto r = (lua_pcall(m_L, m_narg, (m_nres == 0 ? LUA_MULTRET : m_nres), 0) == LUA_OK);
		return r;
	}


	std::string getString(LuaStack& lua, const std::string& id, std::string valueIfMissing)
	{
		lua.pull(id);
		if( lua.isString() )
		{
			valueIfMissing = lua.toString();
		}
		lua.pop();
		return valueIfMissing;
	}

	double getDouble(LuaStack& lua, const std::string& id, double valueIfMissing)
	{
		lua.pull(id);
		if( lua.isNumber() )
		{
			valueIfMissing = lua.toDouble();
		}
		lua.pop();
		return valueIfMissing;
	}

	float getFloat(LuaStack& lua, const std::string& id, float valueIfMissing)
	{
		lua.pull(id);
		if( lua.isNumber() )
		{
			valueIfMissing = lua.toFloat();
		}
		lua.pop();
		return valueIfMissing;
	}

	uint32_t getUint(LuaStack& lua, const std::string& id, uint32_t valueIfMissing)
	{
		lua.pull(id);
		if( lua.isNumber() )
		{
			valueIfMissing = lua.toUint();
		}
		lua.pop();
		return valueIfMissing;
	}

	int32_t getInt(LuaStack& lua, const std::string& id, int32_t valueIfMissing)
	{
		lua.pull(id);
		if( lua.isNumber() )
		{
			valueIfMissing = lua.toInt();
		}
		lua.pop();
		return valueIfMissing;
	}

	bool getBool(LuaStack& lua, const std::string& id, bool valueIfMissing)
	{
		lua.pull(id);
		if( lua.isBool() )
		{
			valueIfMissing = lua.toBool();
		}
		lua.pop();
		return valueIfMissing;
	}

	char getChar(LuaStack& lua, const std::string& id, char valueIfMissing)
	{
		lua.pull(id);
		if( lua.isString() )
		{
			valueIfMissing = lua.toString().at(0);
		}
		lua.pop();
		return valueIfMissing;
	}

	std::string getString(LuaStack& lua, int32_t index, std::string valueIfMissing)
	{
		lua.pull(index);
		if( lua.isString() )
		{
			valueIfMissing = lua.toString();
		}
		lua.pop();
		return valueIfMissing;
	}

	double getDouble(LuaStack& lua, int32_t index, double valueIfMissing)
	{
		lua.pull(index);
		if( lua.isNumber() )
		{
			valueIfMissing = lua.toDouble();
		}
		lua.pop();
		return valueIfMissing;
	}

	float getFloat(LuaStack& lua, int32_t index, float valueIfMissing)
	{
		lua.pull(index);
		if( lua.isNumber() )
		{
			valueIfMissing = lua.toFloat();
		}
		lua.pop();
		return valueIfMissing;
	}

	uint32_t getUint(LuaStack& lua, int32_t index, uint32_t valueIfMissing)
	{
		lua.pull(index);
		if( lua.isNumber() )
		{
			valueIfMissing = lua.toUint();
		}
		lua.pop();
		return valueIfMissing;
	}

	int32_t getInt(LuaStack& lua, int32_t index, int32_t valueIfMissing)
	{
		lua.pull(index);
		if( lua.isNumber() )
		{
			valueIfMissing = lua.toInt();
		}
		lua.pop();
		return valueIfMissing;
	}

	bool getBool(LuaStack& lua, int32_t index, bool valueIfMissing)
	{
		lua.pull(index);
		if( lua.isBool() )
		{
			valueIfMissing = lua.toBool();
		}
		lua.pop();
		return valueIfMissing;
	}

	char getChar(LuaStack& lua, int32_t index, char valueIfMissing)
	{
		lua.pull(index);
		if( lua.isString() )
		{
			valueIfMissing = lua.toString().at(0);
		}
		lua.pop();
		return valueIfMissing;
	}
}
