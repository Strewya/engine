#pragma once
/********************************************
*	class:	ScriptingSystem
*	usage:
********************************************/
/******* C++ headers *******/
#include <string>
/******* common headers *******/
#include <Scripting/LuaInclude.h>
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
	class ScriptingSystem
	{
	public:
		bool init();
		bool shutdown();

		void loadFile(const char* filename);
		std::string getString(const char* key);
		int32_t getInt(const char* key);
		float getFloat(const char* key);
		bool getBool(const char* key);

#ifdef _DEBUG
		void dumpStack();
#endif

	private:
		void extract(const char* key);

		lua_State* m_luaState;
		std::string m_extractionString;
	};
}