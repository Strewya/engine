#pragma once
/********************************************
*	class:	LuaSystem
*	usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
#include <Scripting/LuaInclude.h>
/******* extra headers *******/


/******* end header inclusion *******/

namespace Core
{
	class ResourceFile;

	class LuaStack
	{
	public:
		LuaStack(lua_State* L);
		void dumpStack();

		bool loadFile(const ResourceFile& file) const;

		uint32_t getTop() const;
		std::string toString(int32_t index = -1) const;
		

		bool isString(int32_t index = -1) const;
		bool isFunction(int32_t index = -1) const;
		bool isTable(int32_t index = -1) const;

		void pop(int32_t howMany = 1) const;

		bool operator()();

		bool operator()(const int32_t& arg);
		bool operator()(int32_t* ret);

	private:
		lua_State* m_L;
		uint32_t m_narg;
		uint32_t m_nres;
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
