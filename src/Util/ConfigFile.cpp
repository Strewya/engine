//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Util/ConfigFile.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Scripting/ScriptingSystem.h>
/******* end headers *******/

namespace Core
{
	ConfigFile::ConfigFile(ScriptingSystem& scripting)
		: m_scripting(&scripting)
	{}

	std::string ConfigFile::getString(const char* key, const char* valueIfNotPresent)
	{
		if(m_scripting->pushString(key))
		{
			auto ret = m_scripting->toString();
			m_scripting->pop();
			return ret;
		}
		return valueIfNotPresent;
	}


	/*std::string asString(DataFile& file, const char* key)
	{
		return (file.fetchValue(key), file.getString());
	}
	int32_t asInt(DataFile& file, const char* key)
	{
		return (file.fetchValue(key), file.getInt());
	}
	double asDouble(DataFile& file, const char* key)
	{
		return (file.fetchValue(key), file.getDouble());
	}
	float asFloat(DataFile& file, const char* key)
	{
		return (file.fetchValue(key), file.getFloat());
	}
	bool asBool(DataFile& file, const char* key)
	{
		return (file.fetchValue(key), file.getBool());
	}
	Vec2 asVec2(DataFile& file, const char* key)
	{
		return (file.fetchValue(key), file.getVec2());
	}

	DataFile::DataFile(lua_State* lua)
		: m_luaState(lua)
	{}

	bool DataFile::open(const char* filename)
	{
		assert(filename != nullptr);
		int32_t ret = luaL_loadfile(m_luaState, filename);
		ret = lua_pcall(m_luaState, 0, LUA_MULTRET, 0);
		return ret == 0;
	}

	void DataFile::close()
	{
		lua_pop(m_luaState, 1);
	}

	bool DataFile::fetchValue(const char* key)
	{
		return extract(m_luaState, key);
	}

	bool DataFile::popValue()
	{
		if(lua_gettop(m_luaState) > 0)
		{
			lua_pop(m_luaState, 1);
			return true;
		}
		return false;
	}

	std::string DataFile::getString()
	{
		if(lua_gettop(m_luaState) > 0)
		{
			std::string ret(lua_tostring(m_luaState, -1));
			lua_pop(m_luaState, 1);
			return ret;
		}
		return "";
	}

	int32_t DataFile::getInt()
	{
		if(lua_gettop(m_luaState) > 0)
		{
			int32_t ret = lua_tointeger(m_luaState, -1);
			lua_pop(m_luaState, 1);
			return ret;
		}
		return 0;
	}

	double DataFile::getDouble()
	{
		if(lua_gettop(m_luaState) > 0)
		{
			double ret = lua_tonumber(m_luaState, -1);
			lua_pop(m_luaState, 1);
			return ret;
		}
		return 0;
	}

	float DataFile::getFloat()
	{
		if(lua_gettop(m_luaState) > 0)
		{
			float ret = (float)lua_tonumber(m_luaState, -1);
			lua_pop(m_luaState, 1);
			return ret;
		}
		return 0;
	}

	bool DataFile::getBool()
	{
		if(lua_gettop(m_luaState) > 0)
		{
			bool ret = lua_toboolean(m_luaState, -1) == 1;
			lua_pop(m_luaState, 1);
			return ret;
		}
		return false;
	}

	Vec2 DataFile::getVec2()
	{
		if(lua_gettop(m_luaState) > 0)
		{
			Vec2 ret;
			if(extract(m_luaState, "x") && lua_isnil(m_luaState, -1))
			{
				lua_pop(m_luaState, 1);
				lua_pushinteger(m_luaState, 1);
				lua_gettable(m_luaState, -2);
			}
			ret.x = (float)lua_tonumber(m_luaState, -1);
			lua_pop(m_luaState, 1);
			if(extract(m_luaState, "y") && lua_isnil(m_luaState, -1))
			{
				lua_pop(m_luaState, 1);
				lua_pushinteger(m_luaState, 2);
				lua_gettable(m_luaState, -2);
			}
			ret.y = (float)lua_tonumber(m_luaState, -1);
			lua_pop(m_luaState, 2);
			return ret;
		}
		return Vec2();
	}

	uint32_t DataFile::getArraySize()
	{
		if(lua_gettop(m_luaState) > 0)
		{
			uint32_t ret = lua_objlen(m_luaState, -1);
			return ret;
		}
		return 0;
	}*/
}
