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

		void executeFile(const char* filename);
		bool fileExists(const char* filename);

		void executeFunction(const char* function, void* objArg, const char* objType);
		bool functionExists(const char* function);

		//pops <count> number of arguments from the stack
		//<count> is 1 by default
		//always succeeds, if <count> is greater than num of elements on stack, all are popped
		void pop(uint32_t count = 1);
		
		//puts the value t[key] onto the stack, where 't' is either the global table (if <global> == true) or the table on the top of the stack
		//<global> is false by default, if <key> is nullptr, function return false
		//returns true if <global> is true and <key> exists globally, or if <global> is false, and there is a table on top and the <key> exists in that table
		bool getString(const char* key, bool global = false);
		//returns the value from the stack top as string, or <valueIfNotPresent> if the value is not a string
		//if argument is nullptr, an empty string is returned
		//always succeeds by either returning the string on top of the stack, the argument value, or empty string if argument is nullptr
		std::string toString(const char* valueIfNotPresent) const;
		
		//puts the value t[key] onto the stack, where 't' is either the global table (if <global> == true) or the table on the top of the stack
		//<global> is false by default, if <key> is nullptr, function return false
		//returns true if <global> is true and <key> exists globally, or if <global> is false, and there is a table on top and the <key> exists in that table
		bool getFloat(const char* key, bool global = false);
		//returns the value from the stack top as float, or <valueIfNotPresent> if the value is not a number
		//always succeeds by either returning the number on top of the stack or the argument value
		float toFloat(float valueIfNotPresent) const;
		
		//puts the value t[key] onto the stack, where 't' is either the global table (if <global> == true) or the table on the top of the stack
		//<global> is false by default, if <key> is nullptr, function return false
		//returns true if <global> is true and <key> exists globally, or if <global> is false, and there is a table on top and the <key> exists in that table
		bool getDouble(const char* key, bool global = false);
		//returns the value from the stack top as double, or <valueIfNotPresent> if the value is not a number
		//always succeeds by either returning the number on top of the stack or the argument value
		double toDouble(double valueIfNotPresent) const;
		
		//puts the value t[key] onto the stack, where 't' is either the global table (if <global> == true) or the table on the top of the stack
		//<global> is false by default, if <key> is nullptr, function return false
		//returns true if <global> is true and <key> exists globally, or if <global> is false, and there is a table on top and the <key> exists in that table
		bool getBool(const char* key, bool global = false);
		//returns the value from the stack top as bool, or <valueIfNotPresent> if the value is not a boolean
		//always succeeds by either returning the boolean on top of the stack or the argument value
		bool toBool(bool valueIfNotPresent) const;

		//puts the value t[key] onto the stack, where 't' is either the global table (if <global> == true) or the table on the top of the stack
		//<global> is false by default, if <key> is nullptr, function return false
		//returns true if <global> is true and <key> exists globally, or if <global> is false, and there is a table on top and the <key> exists in that table
		bool getInt(const char* key, bool global = false);
		//returns the value from the stack top as int32_t, or <valueIfNotPresent> if the value is not a number
		//always succeeds by either returning the number on top of the stack or the argument value
		int32_t toInt(int32_t valueIfNotPresent) const;

		//puts the value t[key] onto the stack, where 't' is either the global table (if <global> == true) or the table on the top of the stack
		//<global> is false by default, if <key> is nullptr, function return false
		//returns true if <global> is true and <key> exists globally, or if <global> is false, and there is a table on top and the <key> exists in that table
		bool getVec2(const char* key, bool global = false);
		//returns the value from the stack top as Vec2, or <valueIfNotPresent> if the value is not a table with 2 numbers
		//the value on top of the stack must be a table with exactly 2 numbers, which are either mapped to 'x' and 'y' keys or a sequence of two numbers
		//always succeeds by either returning the parsed table on top of the stack or the argument value
		Vec2 toVec2(const Vec2& valueIfNotPresent) const;

		//puts the value t[key] onto the stack, where 't' is either the global table (if <global> == true) or the table on the top of the stack
		//<global> is false by default, if <key> is nullptr, function return false
		//returns true if <global> is true and <key> exists globally, or if <global> is false, and there is a table on top and the <key> exists in that table
		bool getColor(const char* key, bool global = false);
		//returns the value from the stack top as Color, or <valueIfNotPresent> if the value is not a table with 3 or 4 numbers
		//the value on top of the stack must be a table with either 3 or 4 numbers, which are either mapped to 'r', 'g', 'b' and 'a' keys,
		//or a sequence of three or four numbers sequenced as r, g, b [, a]
		//the alpha is optional, and defaults to 1
		//always succeeds by either returning the parsed table on top of the stack or the argument value
		Color toColor(const Color& valueIfNotPresent) const;

	private:
		lua_State* m_luaState;
	};
}