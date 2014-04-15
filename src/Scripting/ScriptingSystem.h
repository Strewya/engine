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
#include <Util/Color.h>
#include <Util/Vec2.h>
/******* end header inclusion *******/

namespace Core
{
	class ScriptingSystem
	{
	public:
		bool init();
		bool shutdown();

		uint32_t getTop() const;
		void pop(uint32_t count = 1);

		bool doFile(const char* filename, uint32_t numReturnValues = 0);
		bool functionExists(const char* function);
		bool doFunction(const char* function, void* objArg, const char* objType, uint32_t numReturnValues = 0);

		void push(const char* val);
		void push(int32_t val);

		bool getValue(int32_t key, int32_t table);
		bool getValue(const char* key, int32_t table);

		bool isString() const;
		std::string toString() const;

		bool isNumber() const;
		int32_t toInt() const;
		float toFloat() const;
		double toDouble() const;

		bool isBoolean() const;
		bool toBool() const;

		bool isTable() const;
		uint32_t getObjLen() const;


		/*
		
		bool fileExists(const char* filename);

		void executeFunction(const char* function, void* objArg, const char* objType);
		

		//pops <count> number of arguments from the stack
		//<count> is 1 by default
		//always succeeds, if <count> is greater than num of elements on stack, all are popped
		
		
		//FOR ALL GET FUNCTIONS:
		//puts the value t[key] onto the stack, where 't' is either the global table (if <global> == true) or the table on the top of the stack
		//<global> is false by default, if <key> is nullptr, function return false
		//returns true if <global> is true and <key> exists globally, or if <global> is false, and there is a table on top and the <key> exists in that table
		bool getString(const char* key, bool global = false);
		bool getFloat(const char* key, bool global = false);
		bool getDouble(const char* key, bool global = false);
		bool getInt(const char* key, bool global = false);
		bool getBool(const char* key, bool global = false);
		bool getVec2(const char* key, bool global = false);
		bool getColor(const char* key, bool global = false);

		//FOR ALL TO FUNCTIONS
		//returns the value from the stack top as the specified type, or <valueIfNotPresent> if the value does not match the type
		//specially for strings, if default value is nullptr, an empty string is returned
		//always succeeds by either returning the value on top of the stack or the argument value
		std::string toString(const char* valueIfNotPresent);
		float toFloat(float valueIfNotPresent);
		double toDouble(double valueIfNotPresent);
		bool toBool(bool valueIfNotPresent);
		int32_t toInt(int32_t valueIfNotPresent);
		Vec2 toVec2(const Vec2& valueIfNotPresent);
		Color toColor(const Color& valueIfNotPresent);

		//FOR ALL IS FUNCTIONS
		//checks if the value on top of the stack can be parsed to the specified type
		bool isString();
		bool isNumber();
		bool isBoolean();
		bool isList();
		bool isVec2();
		bool isColor();*/

	private:
		lua_State* m_luaState;
	};
}