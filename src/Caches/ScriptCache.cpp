//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Caches/ScriptCache.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Scripting/LuaSystem.h>
#include <Util/ResourceFile.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	bool ScriptCache::init(LuaSystem& luaSystem)
	{
		bool status = true;

		m_luaSystem = &luaSystem;

		DEBUG_INIT(ScriptCache);
		return status;
	}

	bool ScriptCache::shutdown()
	{
		bool status = true;

		DEBUG_SHUTDOWN(ScriptCache);
		return status;
	}

	LoadResult ScriptCache::load(const ResourceFile& file)
	{
		auto index = valueFind(m_data, file.getPath());
		if( index != m_data.size() )
		{
			return{LoadResultFlag::Success, "Script already loaded"};
		}
		auto lua = m_luaSystem->getStack();
		auto status = lua.doFile(file.getPath().c_str());
		if( !status )
		{
			auto str = lua.toString();
			lua.pop();
			lua.pull("global_dependency_missing", 0);
			if( lua.isString() )
			{
				auto str = lua.toString();
				lua.pop();
				lua.push();
				lua.setValue("global_dependency_missing", 0);
				return{LoadResultFlag::DependencyMissing, str};
			}
			lua.pop();
			return{LoadResultFlag::Fail, str};
		}
		m_data.emplace_back(file.getPath());
		lua.pull("global_dependency_table", 0);
		lua.push(true);
		lua.setValue(file.getName(), -2);
		lua.pop(); //table
		return{LoadResultFlag::Success};
	}


	LoadResult ScriptCache::reload(const ResourceFile& file)
	{
		auto index = valueFind(m_data, file.getPath());
		if( index == m_data.size() )
		{
			return{LoadResultFlag::Fail, "Script not loaded"};
		}
		auto lua = m_luaSystem->getStack();
		auto status = lua.doFile(file.getPath().c_str());
		if( !status )
		{
			auto str = lua.toString();
			lua.pop();
			lua.pull("global_dependency_missing", 0);
			if( lua.isString() )
			{
				auto str = lua.toString();
				lua.pop();
				lua.push();
				lua.setValue("global_dependency_missing", 0);
				return{LoadResultFlag::DependencyMissing, str};
			}
			lua.pop();
			return{LoadResultFlag::Fail, str};
		}
		return{LoadResultFlag::Success};
	}

	LoadResult ScriptCache::unload(const ResourceFile& file)
	{
		auto index = valueFind(m_data, file.getPath());
		m_data.erase(m_data.begin() + index);
		return{LoadResultFlag::Success};
	}
}
