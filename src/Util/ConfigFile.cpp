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
		: m_scripting(&scripting), m_isOpen(false), m_filename("")
	{}

	bool ConfigFile::open(const char* filename)
	{
		assert(filename != nullptr);
		uint32_t resetPoint = m_scripting->getTop();
		if(m_isOpen == false && m_scripting->doFile(filename, 1))
		{
			m_isOpen = true;
			return true;
		}
		m_scripting->pop(m_scripting->getTop() - resetPoint);
		return false;
	}

	void ConfigFile::close()
	{
		if(m_isOpen)
		{
			m_scripting->pop(1);
			m_isOpen = false;
		}
	}

	const std::string& ConfigFile::getFilename() const
	{
		return m_filename;
	}

	std::string ConfigFile::getString(const char* key, const char* valueIfNotPresent)
	{
		assert(valueIfNotPresent);
		std::string ret = valueIfNotPresent;
		if(m_scripting->getValue(key, -1))
		{
			if(m_scripting->isString())
			{
				ret = m_scripting->toString();
			}
			m_scripting->pop();
		}
		return ret;
	}

	int32_t ConfigFile::getInt(const char* key, int32_t valueIfNotPresent)
	{
		if(m_scripting->getValue(key, -1))
		{
			if(m_scripting->isNumber())
			{
				valueIfNotPresent = m_scripting->toInt();
			}
			m_scripting->pop();
		}
		return valueIfNotPresent;
	}

	float ConfigFile::getFloat(const char* key, float valueIfNotPresent)
	{
		if(m_scripting->getValue(key, -1))
		{
			if(m_scripting->isNumber())
			{
				valueIfNotPresent = m_scripting->toFloat();
			}
			m_scripting->pop();
		}
		return valueIfNotPresent;
	}

	Vec2 ConfigFile::getVec2(const char* key, Vec2 valueIfNotPresent)
	{
		if(m_scripting->getValue(key, -1))
		{
			if(m_scripting->isTable())
			{
				auto top = m_scripting->getTop();
				if((m_scripting->getValue("x", -1) || m_scripting->getValue(1, -1)) &&
				   (m_scripting->getValue("y", -2) || m_scripting->getValue(2, -2)))
				{
					if(m_scripting->isNumber())
					{
						valueIfNotPresent.y = m_scripting->toFloat();
					}
					m_scripting->pop();
					if(m_scripting->isNumber())
					{
						valueIfNotPresent.x = m_scripting->toFloat();
					}
					m_scripting->pop();
				}
				m_scripting->pop(m_scripting->getTop() - top);
			}
			m_scripting->pop();
		}
		return valueIfNotPresent;
	}

	bool ConfigFile::getListElement(const char* list, uint32_t element)
	{
		if(m_scripting->getValue(list, -1))
		{
			if(m_scripting->getValue(element, -1))
			{
				return true;
			}
		}
		return false;
	}

	uint32_t ConfigFile::getListSize(const char* list)
	{
		uint32_t size = 0;
		if(m_scripting->getValue(list, -1))
		{
			size = m_scripting->getObjLen();
			m_scripting->pop();
		}
		return size;
	}

	void ConfigFile::popListElement()
	{
		m_scripting->pop(2);
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
