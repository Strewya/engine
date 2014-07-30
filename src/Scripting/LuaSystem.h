#pragma once
/********************************************
*	class:	LuaSystem
*	usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
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
		template<typename ...Args> bool call(int32_t arg, Args... rest)
		{
			lua_pushinteger(m_L, arg);
			++m_narg;
			auto res = call(rest...);
			--m_narg;
			return res;
		}
		template<typename ...Args> bool call(uint32_t arg, Args... rest)
		{
			lua_pushinteger(m_L, arg);
			++m_narg;
			auto res = call(rest...);
			--m_narg;
			return res;
		}
		template<typename ...Args> bool call(bool arg, Args... rest)
		{
			lua_pushboolean(m_L, arg);
			++m_narg;
			auto res = call(rest...);
			--m_narg;
			return res;
		}
		template<typename ...Args> bool call(float arg, Args... rest)
		{
			lua_pushnumber(m_L, arg);
			++m_narg;
			auto res = call(rest...);
			--m_narg;
			return res;
		}
		template<typename ...Args> bool call(double arg, Args... rest)
		{
			lua_pushnumber(m_L, arg);
			++m_narg;
			auto res = call(rest...);
			--m_narg;
			return res;
		}
		template<typename ...Args> bool call(char arg, Args... rest)
		{
			lua_pushstring(m_L, arg);
			++m_narg;
			auto res = call(rest...);
			--m_narg;
			return res;
		}
		template<typename ...Args> bool call(const std::string& arg, Args... rest)
		{
			lua_pushstring(m_L, arg.c_str());
			++m_narg;
			auto res = call(rest...);
			--m_narg;
			return res;
		}
		template<typename ...Args> bool call(const CustomType& arg, Args... rest)
		{
			tolua_pushusertype(m_L, arg.ptr, arg.name);
			++m_narg;
			auto res = call(rest...);
			--m_narg;
			return res;
		}
		template<typename ...Args> bool call(int32_t* arg, Args... rest)
		{
			++m_nres;
			auto res = call(rest...);
			*arg = toInt();
			pop();
			--m_nres;
			return res;
		}
		template<typename ...Args> bool call(uint32_t* arg, Args... rest)
		{
			++m_nres;
			auto res = call(rest...);
			*arg = toUint();
			pop();
			--m_nres;
			return res;
		}
		template<typename ...Args> bool call(bool* arg, Args... rest)
		{
			++m_nres;
			auto res = call(rest...);
			*arg = toBool();
			pop();
			--m_nres;
			return res;
		}
		template<typename ...Args> bool call(float* arg, Args... rest)
		{
			++m_nres;
			auto res = call(rest...);
			*arg = toFloat();
			pop();
			--m_nres;
			return res;
		}
		template<typename ...Args> bool call(double* arg, Args... rest)
		{
			++m_nres;
			auto res = call(rest...);
			*arg = toDouble();
			pop();
			--m_nres;
			return res;
		}
		template<typename ...Args> bool call(std::string* arg, Args... rest)
		{
			++m_nres;
			auto res = call(rest...);
			*arg = toString();
			pop();
			--m_nres;
			return res;
		}


	private:
		lua_State* m_L;
		uint32_t m_narg;
		uint32_t m_nres;
		struct Iteration
		{
			int32_t m_iterateTableIndex;
			bool m_iterateAll;
		};
		std::vector<Iteration> m_iters;
	};




	class LuaSystem
	{
	public:
		bool init();
		bool shutdown();
		void update();

		LuaStack getStack() const;


	private:
		lua_State* m_L;
	};


	void runPlayground();
}
