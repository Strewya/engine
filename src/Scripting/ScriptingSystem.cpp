//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Scripting/ScriptingSystem.h>
/******* C++ headers *******/
#include <cassert>
#ifdef _DEBUG
#include <iostream>
#endif
/******* extra headers *******/
#include <Scripting/luaBinding.h>
#include <Util/Dimensional.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	static bool extract(lua_State* lua, const char* key)
	{
		std::string extractStatement("local data = ...; return data.");
		luaL_loadstring(lua, (extractStatement + key).c_str());
		lua_pushvalue(lua, -2);
		return lua_pcall(lua, 1, 1, 0) == 0;
	}

	static bool extractFunction(lua_State* lua, const char* func)
	{
		std::string extractStatement("return ");
		luaL_loadstring(lua, (extractStatement + func).c_str());
		return lua_pcall(lua, 0, 1, 0) == 0;
	}

#ifdef _DEBUG
	void dumpStack(lua_State* lua)
	{
		uint32_t top = lua_gettop(lua);
		for(uint32_t i = 1; i <= top; ++i)
		{  /* repeat for each level */
			int t = lua_type(lua, i);
			switch(t)
			{
			case LUA_TSTRING:  /* strings */
			std::cout << lua_tostring(lua, i);
			break;

			case LUA_TBOOLEAN:  /* booleans */
			std::cout << lua_toboolean(lua, i) ? "true" : "false";
			break;

			case LUA_TNUMBER:  /* numbers */
			std::cout << lua_tonumber(lua, i);
			break;

			default:  /* other values */
			std::cout << lua_typename(lua, t);
			break;

			}
			std::cout << " ";
		}
		std::cout << std::endl;
	}
#endif

	static const char* codeName(uint32_t code)
	{
		static const char* names[] = {"OK", "yield", "runtime error", "syntax error", "memory alloc error", "error handler error"};
		return names[code];
	}

	bool ScriptingSystem::init()
	{
		bool status = false;
		m_luaState = luaL_newstate();
		if(m_luaState)
		{
			luaL_openlibs(m_luaState);
			tolua_core_open(m_luaState);
			status = true;
		}
		DEBUG_INFO("ScriptingSystem init ", status ? "OK" : "FAIL");
		return status;
	}

	bool ScriptingSystem::shutdown()
	{
		lua_close(m_luaState);
		return true;
	}

	DataFile ScriptingSystem::getDataFile(const char* filename)
	{
		DataFile df(m_luaState);
		if(filename != nullptr)
			df.open(filename);
		return df;
	}

	void ScriptingSystem::executeScriptFile(const char* scriptName)
	{
		int32_t ret = luaL_loadfile(m_luaState, scriptName);
		DEBUG_LINE(if(ret != 0) { DEBUG_INFO(scriptName, " loading: ", codeName(ret)); });
		ret = lua_pcall(m_luaState, 0, 0, 0);
		DEBUG_LINE(if(ret != 0) { std::string msg = tolua_tostring(m_luaState, 1, 0); DEBUG_INFO(scriptName, " execution:\n", codeName(ret), " - ", msg); });
	}

	void ScriptingSystem::executeFunction(const char* function, void* objArg, const char* objType)
	{
		assert(function != nullptr && objArg != nullptr && objType != nullptr);
		if(extractFunction(m_luaState, function) && lua_isfunction(m_luaState, -1))
		{
			tolua_pushusertype(m_luaState, objArg, objType);
			int32_t ret = lua_pcall(m_luaState, 1, 0, 0);
			assert(ret == 0);
		}
	}

	bool ScriptingSystem::functionExists(const char* function)
	{
		assert(function != nullptr);
		auto exists = false;
		if(extractFunction(m_luaState, function))
		{
			exists = true;
			lua_pop(m_luaState, 1);
		}
		return exists;
	}

	bool ScriptingSystem::scriptFileExists(const char* scriptName)
	{
		assert(scriptName != nullptr);
		int32_t ret = luaL_loadfile(m_luaState, scriptName);
		if(ret == 0)
		{
			lua_pop(m_luaState, 1);
		}
		return ret == 0;
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

	std::string DataFile::getString(const char* key)
	{
		if(extract(m_luaState, key))
		{
			std::string ret(lua_tostring(m_luaState, -1));
			lua_pop(m_luaState, 1);
			return ret;
		}
		return "";
	}

	int32_t DataFile::getInt(const char* key)
	{
		if(extract(m_luaState, key))
		{
			int32_t ret = lua_tointeger(m_luaState, -1);
			lua_pop(m_luaState, 1);
			return ret;
		}
		return 0;
	}

	double DataFile::getDouble(const char* key)
	{
		if(extract(m_luaState, key))
		{
			double ret = lua_tonumber(m_luaState, -1);
			lua_pop(m_luaState, 1);
			return ret;
		}
		return 0;
	}

	float DataFile::getFloat(const char* key)
	{
		if(extract(m_luaState, key))
		{
			float ret = (float)lua_tonumber(m_luaState, -1);
			lua_pop(m_luaState, 1);
			return ret;
		}
		return 0;
	}

	bool DataFile::getBool(const char* key)
	{
		if(extract(m_luaState, key))
		{
			bool ret = lua_toboolean(m_luaState, -1) == 1;
			lua_pop(m_luaState, 1);
			return ret;
		}
		return false;
	}
}
/*
namespace Script
{
	
	void Engine::ClearStack()
	{
		lua_pop(_lua, lua_gettop(_lua));
	}

	int Engine::GetStackSize()
	{
		return lua_gettop(_lua);
	}

	int Engine::GetResultCount()
	{
		return _nresults;
	}

	bool Engine::DoFile(const std::string& filename)
	{
		return DoFile(filename.c_str());
	}

	bool Engine::DoFile(const char* filename)
	{
		int result = luaL_dofile(_lua, filename);
		if(result != 0)
		{
			HandleError(filename, result);
			return false;
		}
		return true;
	}
	bool Engine::DoString(const std::string& script)
	{
		return DoString(script.c_str());
	}

	bool Engine::DoString(const char* script)
	{
		int result = luaL_dostring(_lua, script);
		if(result != 0)
		{
			HandleError(script, result);
			return false;
		}
		return true;
	}
	bool Engine::Push(const int& in)
	{
		lua_pushinteger(_lua, in);
		return true;
	}

	bool Engine::Push(const uint32_t& in)
	{
		lua_pushinteger(_lua, in);
		return true;
	}

	bool Engine::Push(const std::string& in)
	{
		lua_pushstring(_lua, in.c_str());
		return true;
	}

	bool Engine::Push(const char* in)
	{
		lua_pushstring(_lua, in);
		return true;
	}

	bool Engine::Push(const bool& in)
	{
		lua_pushboolean(_lua, in);
		return true;
	}

	bool Engine::Push(const double& in)
	{
		lua_pushnumber(_lua, in);
		return true;
	}

	bool Engine::Push(const float& in)
	{
		lua_pushnumber(_lua, in);
		return true;
	}

	bool Engine::Push(const Util::Vec2& in)
	{
		lua_newtable(_lua);
		int table = GetStackSize();
		Push("x");
		Push(in.x);
		lua_settable(_lua, table);
		Push("y");
		Push(in.y);
		lua_settable(_lua, table);
		return true;
	}

	bool Engine::Push(const Util::Vec3& in)
	{
		lua_newtable(_lua);
		int table = GetStackSize();
		Push("x");
		Push(in.x);
		lua_settable(_lua, table);
		Push("y");
		Push(in.y);
		lua_settable(_lua, table);
		Push("z");
		Push(in.z);
		lua_settable(_lua, table);
		return true;
	}

	bool Engine::Push(const Util::Rect& in)
	{
		lua_newtable(_lua);
		int table = GetStackSize();
		Push("x");
		Push(in.position.x);
		lua_settable(_lua, table);
		Push("y");
		Push(in.position.y);
		lua_settable(_lua, table);
		Push("w");
		Push(in.width);
		lua_settable(_lua, table);
		Push("h");
		Push(in.height);
		lua_settable(_lua, table);
		return true;
	}

	bool Engine::Push(const Util::Color& in)
	{
		lua_newtable(_lua);
		int table = GetStackSize();
		Push("r");
		Push(in.getRed());
		lua_settable(_lua, table);
		Push("g");
		Push(in.getGreen());
		lua_settable(_lua, table);
		Push("b");
		Push(in.getBlue());
		lua_settable(_lua, table);
		Push("a");
		Push(in.getAlpha());
		lua_settable(_lua, table);
		return true;
	}
	bool Engine::Pop(int& out, int index)
	{
		if(lua_gettop(_lua) != 0 && lua_isnumber(_lua, index))
		{
			out = lua_tointeger(_lua, index);
			lua_remove(_lua, index);
			return true;
		}
		return false;
	}

	bool Engine::Pop(uint32_t& out, int index)
	{
		if(lua_gettop(_lua) != 0 && lua_isnumber(_lua, index))
		{
			out = lua_tointeger(_lua, index);
			lua_remove(_lua, index);
			return true;
		}
		return false;
	}

	bool Engine::Pop(bool& out, int index)
	{
		if(lua_gettop(_lua) != 0 && lua_isboolean(_lua, index))
		{
			out = lua_toboolean(_lua, index) != 0;
			lua_remove(_lua, index);
			return true;
		}
		return false;
	}

	bool Engine::Pop(std::string& out, int index)
	{
		if(lua_gettop(_lua) != 0 && lua_isstring(_lua, index))
		{
			out = lua_tostring(_lua, index);
			lua_remove(_lua, index);
			return true;
		}
		return false;
	}

	bool Engine::Pop(double& out, int index)
	{
		if(lua_gettop(_lua) != 0 && lua_isnumber(_lua, index))
		{
			out = lua_tonumber(_lua, index);
			lua_remove(_lua, index);
			return true;
		}
		return false;
	}

	bool Engine::Pop(float& out, int index)
	{
		if(lua_gettop(_lua) != 0 && lua_isnumber(_lua, index))
		{
			out = (float)lua_tonumber(_lua, index);
			lua_remove(_lua, index);
			return true;
		}
		return false;
	}

	bool Engine::Pop(Util::Vec2& out, int index)
	{
		if(lua_gettop(_lua) != 0 && lua_istable(_lua, index))
		{
			if(GetField("x", -1) && Pop(out.x) && GetField("y", -1) && Pop(out.y))
			{
				lua_remove(_lua, index);
				return true;
			}
		}
		return false;
	}

	bool Engine::Pop(Util::Vec3& out, int index)
	{
		if(lua_gettop(_lua) != 0 && lua_istable(_lua, index))
		{
			if(GetField("x", -1) && Pop(out.x) && GetField("y", -1) && Pop(out.y) && GetField("z", -1) && Pop(out.z))
			{
				lua_remove(_lua, index);
				return true;
			}
		}
		return false;
	}

	bool Engine::Pop(Util::Rect& out, int index)
	{
		if(lua_gettop(_lua) != 0 && lua_istable(_lua, index))
		{
			if(GetField("x", -1) && Pop(out.position.x) && GetField("y", -1) && Pop(out.position.y) && GetField("w", -1) && Pop(out.width) && GetField("h", -1) && Pop(out.height))
			{
				lua_remove(_lua, index);
				return true;
			}
		}
		return false;
	}

	bool Engine::Pop(Util::Color& out, int index)
	{
		if(lua_gettop(_lua) != 0 && lua_istable(_lua, index))
		{
			float r, g, b, a;
			if(GetField("r", -1) && Pop(r) && GetField("g", -1) && Pop(g) && GetField("b", -1) && Pop(b))
			{
				out.setRGBA(r, g, b);
				if(GetField("a", -1) && Pop(a))
				{
					out.setAlpha(a);
				}
				lua_remove(_lua, index);
				return true;
			}
		}
		return false;
	}

	bool Engine::Pop()
	{
		return RemoveElement(-1);
	}

	bool Engine::RemoveElement(int index)
	{
		if(lua_gettop(_lua) == 0 || GetStackSize() < abs(index))
		{
			return false;
		}
		lua_remove(_lua, index);
		return true;
	}

	bool Engine::GetObjLength(int& out, int index)
	{
		if(lua_gettop(_lua) != 0 && (lua_isstring(_lua, index) || lua_istable(_lua, index)))
		{
			out = lua_objlen(_lua, index);
			return true;
		}
		return false;
	}

	bool Engine::GetField(const char* name, int tableIndex)
	{
		std::string value = name;
		uint32_t pos = value.find('.');
		if(pos == value.npos)
		{
			lua_getfield(_lua, tableIndex, name);
		}
		else
		{
			if(!GetField(value.substr(0, pos).c_str(), tableIndex) || !GetField(value.substr(pos+1).c_str(), -1))
			{
				return false;
			}
			RemoveElement(-2);
		}
		return !lua_isnil(_lua, -1);
	}

	bool Engine::GetField(int tableIndex)
	{
		if(lua_gettop(_lua) == 0)
		{
			return false;
		}
		lua_gettable(_lua, tableIndex);
		return !lua_isnil(_lua, -1);
	}
	bool Engine::CallFunction(int arguments, int expectedResults)
	{
		if(lua_isfunction(_lua, -arguments-1))
		{
			int bottom = GetStackSize() - arguments - 1;
			int result = lua_pcall(_lua, arguments, expectedResults, 0);
			if(result == 0)
			{
				_nresults = GetStackSize() - bottom;
				return true;
				
			}
			HandleError("function call", result);
		}
		return false;
	}

	void Engine::HandleError(const char* name, int result)
	{
		Util::Logger& log = Util::GetDefaultLogger();
		log << "Error for " << name << ": ";
		if(result == LUA_ERRRUN)
			log << "runtime error";
		else if(result == LUA_ERRMEM)
			log << "memory alloc error";
		else if(result == LUA_ERRERR)
			log << "error running the error handler function";
		else if(result == LUA_ERRSYNTAX)
			log << "syntax error";
		else if(result == LUA_ERRFILE)
			log << "cannot open/read file";
		log << Util::Logger::endl;
		log << "\t" << lua_tostring(_lua, -1) << Util::Logger::endl;
		lua_remove(_lua, -1);
	}
}

	
		config.lua

testInt = 10;
testString = "test string";
testBool = false;
testFloat = 3.141;
class = {};
class.x = 20;
class.name = "marko";
class.pos = {};
class.pos.x = 7;
class.pos.y = 5;
class.pos.rnd = {};
class.pos.rnd.bla = 11;


function changeName()
	class.name = "MIRKO";
end;

function setName(name)
	class.name = name;
end;

function get_3_Sums(a, b, c)
	return a+b, b+c, a+c;
end;

function class.move(self, x, y)
	self.pos.x = self.pos.x + x;
	self.pos.y = self.pos.y + y;
end;


		TESTS

	SG::Engine script;
	int number, num2, num3, x, y;
	bool bul;
	double pi;
	std::string str, name="Josko";

	assert(script.DoFile("config.lua"));
	
	assert(script.GetValue("testInt", number));
	assert(number == 10);

	assert(script.GetValue("testString", str));
	assert(str == "test string");

	assert(script.GetValue("testBool", bul));
	assert(bul == false);

	assert(script.GetValue("testFloat", pi));
	assert(pi == 3.141);

	assert(script.GetValue("class.x", number));
	assert(number == 20);

	assert(script.GetValue("class.pos.y", number));
	assert(number == 5);

	assert(script.GetValue("class.pos.rnd.bla", number));
	assert(number == 11);

	assert(script.GetValue("class.name", str));
	assert(str == "marko");

	assert(script.GetField("changeName"));
	assert(script.CallFunction(0));
	assert(script.GetValue("class.name", str));
	assert(str == "MIRKO");

	assert(script.GetField("setName"));
	assert(script.Push(name));
	assert(script.CallFunction(1));
	assert(script.GetValue("class.name", str));
	assert(str == name);

	assert(script.GetField("get_3_Sums"));
	assert(script.Push(15));
	assert(script.Push(20));
	assert(script.Push(10));
	assert(script.CallFunction(3));
	assert(script.Pop(number, -3));
	assert(script.Pop(num2, -2));
	assert(script.Pop(num3));
	assert(number == (15+20));
	assert(num2 == (20+10));
	assert(num3 == (15+10));



	assert(script.GetValue("class.pos.x", x));
	assert(script.GetValue("class.pos.y", y));
	assert(script.GetField("class.move"));
	assert(script.GetField("class"));
	assert(script.Push(3));
	assert(script.Push(5));
	assert(script.CallFunction(3));
	assert(script.GetValue("class.pos.x", number));
	assert(number == x+3);
	assert(script.GetValue("class.pos.y", number));
	assert(number == y+5);
*/
