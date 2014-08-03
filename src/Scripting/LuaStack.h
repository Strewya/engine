#pragma once
/********************************************
*	class:	LuaStack
*	usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <string>
#include <vector>
/******* common headers *******/
#include <Scripting/LuaInclude.h>
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
	class ResourceFile;

	struct CustomType
	{
		void* ptr;
		const char* name;
	};

	class LuaStack
	{
	public:
		LuaStack(lua_State* L);
		void dumpStack();

		bool loadFile(const ResourceFile& file);
		bool doFile(const ResourceFile& file);

		uint32_t getTop();
		void pop(int32_t howMany = 1);
		void pull(int32_t key);
		void pull(const std::string& key);

		void pairs(int32_t index = -1);
		void pairs(const std::string& table);
		void ipairs(int32_t index = -1);
		void ipairs(const std::string& table);

		bool next();

		std::string toString(int32_t index = -1);
		uint32_t toUint(int32_t index = -1);
		int32_t toInt(int32_t index = -1);
		float toFloat(int32_t index = -1);
		double toDouble(int32_t index = -1);
		bool toBool(int32_t index = -1);

		bool isNil(int32_t index = -1);
		bool isString(int32_t index = -1);
		bool isFunction(int32_t index = -1);
		bool isTable(int32_t index = -1);
		bool isNumber(int32_t index = -1);
		bool isBool(int32_t index = -1);

		bool call();
		template<typename ...Args> bool call(int32_t arg, Args... rest);
		template<typename ...Args> bool call(uint32_t arg, Args... rest);
		template<typename ...Args> bool call(bool arg, Args... rest);
		template<typename ...Args> bool call(float arg, Args... rest);
		template<typename ...Args> bool call(double arg, Args... rest);
		template<typename ...Args> bool call(char arg, Args... rest);
		template<typename ...Args> bool call(const std::string& arg, Args... rest);
		template<typename ...Args> bool call(const CustomType& arg, Args... rest);
		template<typename ...Args> bool call(int32_t* arg, Args... rest);
		template<typename ...Args> bool call(uint32_t* arg, Args... rest);
		template<typename ...Args> bool call(bool* arg, Args... rest);
		template<typename ...Args> bool call(float* arg, Args... rest);
		template<typename ...Args> bool call(double* arg, Args... rest);
		template<typename ...Args> bool call(std::string* arg, Args... rest);


	private:
		lua_State* m_L;
		uint32_t m_narg;
		uint32_t m_nres;
		struct Iteration
		{
			int32_t m_iterateTableIndex;
			bool m_iterateAll;
			bool m_autoPopTable;
		};
		std::vector<Iteration> m_iters;
	};

	std::string getString(LuaStack& lua, const std::string& id, std::string valueIfMissing);
	double getDouble(LuaStack& lua, const std::string& id, double valueIfMissing);
	float getFloat(LuaStack& lua, const std::string& id, float valueIfMissing);
	uint32_t getUint(LuaStack& lua, const std::string& id, uint32_t valueIfMissing);
	int32_t getInt(LuaStack& lua, const std::string& id, int32_t valueIfMissing);
	bool getBool(LuaStack& lua, const std::string& id, bool valueIfMissing);
	char getChar(LuaStack& lua, const std::string& id, char valueIfMissing);

	std::string getString(LuaStack& lua, int32_t index, std::string valueIfMissing);
	double getDouble(LuaStack& lua, int32_t index, double valueIfMissing);
	float getFloat(LuaStack& lua, int32_t index, float valueIfMissing);
	uint32_t getUint(LuaStack& lua, int32_t index, uint32_t valueIfMissing);
	int32_t getInt(LuaStack& lua, int32_t index, int32_t valueIfMissing);
	bool getBool(LuaStack& lua, int32_t index, bool valueIfMissing);
	char getChar(LuaStack& lua, int32_t index, char valueIfMissing);
}

#include <Scripting/LuaStackImpl.h>
