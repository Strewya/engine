//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Util/DataFile.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Scripting/ScriptingSystem.h>
/******* end headers *******/

namespace Core
{
	void fillValue(ScriptingSystem& script, std::string& v);
	void fillValue(ScriptingSystem& script, int32_t& v);
	void fillValue(ScriptingSystem& script, float& v);
	void fillValue(ScriptingSystem& script, bool& v);
	void fillValue(ScriptingSystem& script, Vec2& v);


	DataFile::DataFile(ScriptingSystem& scripting)
		: m_scripting(&scripting), m_isOpen(false), m_filename("")
	{}

	bool DataFile::open(const char* filename)
	{
		assert(filename != nullptr);
		uint32_t resetPoint = m_scripting->getTop();
		if(m_isOpen == false && m_scripting->doFile(filename, 1))
		{
			m_isOpen = true;
			m_filename.assign(filename);
			return true;
		}
		m_scripting->pop(m_scripting->getTop() - resetPoint);
		return false;
	}

	void DataFile::close()
	{
		if(m_isOpen)
		{
			m_scripting->pop(1);
			m_isOpen = false;
			m_filename.clear();
		}
	}

	const std::string& DataFile::getFilename() const
	{
		return m_filename;
	}

	std::string DataFile::getString(const char* key, const char* defaultValue)
	{
		assert(defaultValue);
		std::string ret(defaultValue);
		if(m_scripting->getValue(key, -1))
		{
			fillValue(*m_scripting, ret);
			m_scripting->pop();
		}
		return ret;
	}

	std::string DataFile::getString(uint32_t index, const char* defaultValue)
	{
		assert(defaultValue);
		std::string ret(defaultValue);
		if(m_scripting->getValue(index, -1))
		{
			fillValue(*m_scripting, ret);
			m_scripting->pop();
		}
		return ret;
	}

	int32_t DataFile::getInt(const char* key, int32_t defaultValue)
	{
		if(m_scripting->getValue(key, -1))
		{
			fillValue(*m_scripting, defaultValue);
			m_scripting->pop();
		}
		return defaultValue;
	}

	int32_t DataFile::getInt(uint32_t index, int32_t defaultValue)
	{
		if(m_scripting->getValue(index, -1))
		{
			fillValue(*m_scripting, defaultValue);
			m_scripting->pop();
		}
		return defaultValue;
	}

	float DataFile::getFloat(const char* key, float defaultValue)
	{
		if(m_scripting->getValue(key, -1))
		{
			fillValue(*m_scripting, defaultValue);
			m_scripting->pop();
		}
		return defaultValue;
	}

	float DataFile::getFloat(uint32_t index, float defaultValue)
	{
		if(m_scripting->getValue(index, -1))
		{
			fillValue(*m_scripting, defaultValue);
			m_scripting->pop();
		}
		return defaultValue;
	}

	bool DataFile::getBool(const char* key, bool defaultValue)
	{
		if(m_scripting->getValue(key, -1))
		{
			fillValue(*m_scripting, defaultValue);
			m_scripting->pop();
		}
		return defaultValue;
	}

	bool DataFile::getBool(uint32_t index, bool defaultValue)
	{
		if(m_scripting->getValue(index, -1))
		{
			fillValue(*m_scripting, defaultValue);
			m_scripting->pop();
		}
		return defaultValue;
	}

	Vec2 DataFile::getVec2(const char* key, Vec2 defaultValue)
	{
		if(m_scripting->getValue(key, -1))
		{
			fillValue(*m_scripting, defaultValue);
			m_scripting->pop();
		}
		return defaultValue;
	}

	Vec2 DataFile::getVec2(uint32_t index, Vec2 defaultValue)
	{
		if(m_scripting->getValue(index, -1))
		{
			fillValue(*m_scripting, defaultValue);
			m_scripting->pop();
		}
		return defaultValue;
	}

	bool DataFile::getList(const char* list)
	{
		if(m_scripting->getValue(list, -1))
		{
			return true;
		}
		return false;
	}

	bool DataFile::getList(uint32_t index)
	{
		if(m_scripting->getValue(index, -1))
		{
			if(m_scripting->isTable())
			{
				return true;
			}
			m_scripting->pop();
		}
		return false;
	}

	uint32_t DataFile::getListSize(const char* list)
	{
		uint32_t size = 0;
		if(m_scripting->getValue(list, -1))
		{
			size = m_scripting->getObjLen();
			m_scripting->pop();
		}
		return size;
	}

	void DataFile::popList()
	{
		m_scripting->pop(1);
	}

	void fillValue(ScriptingSystem& script, std::string& v)
	{
		if(script.isString())
		{
			v = script.toString();
		}
	}

	void fillValue(ScriptingSystem& script, int32_t& v)
	{
		if(script.isNumber())
		{
			v = script.toInt();
		}
	}

	void fillValue(ScriptingSystem& script, float& v)
	{
		if(script.isNumber())
		{
			v = script.toFloat();
		}
	}

	void fillValue(ScriptingSystem& script, bool& v)
	{
		if(script.isBoolean())
		{
			v = script.toBool();
		}
	}

	void fillValue(ScriptingSystem& script, Vec2& v)
	{
		if(script.isTable())
		{
			auto top = script.getTop();
			if((script.getValue("x", -1) || script.getValue(1, -1)) &&
			   (script.getValue("y", -2) || script.getValue(2, -2)))
			{
				if(script.isNumber())
				{
					v.y = script.toFloat();
				}
				script.pop();
				if(script.isNumber())
				{
					v.x = script.toFloat();
				}
				script.pop();
			}
			script.pop(script.getTop() - top);
		}
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
