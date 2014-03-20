#pragma once
/********************************************
*	class:	ScriptingSystem
*	usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <string>
#include <vector>
/******* common headers *******/
#include <Scripting/LuaInclude.h>
/******* extra headers *******/
#include <Util/Vec2.h>
/******* end header inclusion *******/

namespace Core
{
	class ScriptingSystem
	{
	public:
		bool init();
		bool shutdown();

		void executeFile(const char* filename);
		bool fileExists(const char* filename);

		void executeFunction(const char* function, void* objArg, const char* objType);
		bool functionExists(const char* function);

		//value operations
		void pop();
		bool fetchString(const char* key);
		std::string toString(const char* valueIfNotPresent) const;

	private:
		lua_State* m_luaState;
	};
}