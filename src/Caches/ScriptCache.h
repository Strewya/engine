#pragma once
/********************************************
*	class:	ScriptCache
*	usage:
********************************************/
/******* C++ headers *******/
#include <string>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <Loaders/Defines.h>
/******* end header inclusion *******/

namespace Core
{
	class ResourceFile;
	class LuaSystem;

	class ScriptCache
	{
	public:
		bool init(LuaSystem& scripting);
		bool shutdown();

		LoadResult load(const ResourceFile& file);
		LoadResult reload(const ResourceFile& file);
		LoadResult unload(const ResourceFile& file);

	private:
		LuaSystem* m_luaSystem;
		std::vector<std::string> m_data;
	};
}
