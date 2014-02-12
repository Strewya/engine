#pragma once
/********************************************
*	class:	ScriptingSystem
*	usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <string>
/******* common headers *******/
#include <Scripting/LuaInclude.h>
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
	class DataFile
	{
	public:
		DataFile(lua_State* state);

		bool open(const char* filename);
		void close();

		std::string getString(const char* key);
		int32_t getInt(const char* key);
		double getDouble(const char* key);
		float getFloat(const char* key);
		bool getBool(const char* key);

	private:
		lua_State* m_luaState;
	};

	class ScriptingSystem
	{
	public:
		bool init();
		bool shutdown();

		DataFile getDataFile(const char* filename = nullptr);

		void executeScriptFile(const char* scriptName);
		void executeFunction(const char* function, void* objArg, const char* objType);

		bool functionExists(const char* function);
		bool scriptFileExists(const char* scriptName);

	private:
		lua_State* m_luaState;
	};
}