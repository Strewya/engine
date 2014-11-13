//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Caches/AnimationCache.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Caches/ImageCache.h>
#include <Scripting/LuaStack.h>
#include <Util/ResourceFile.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	bool AnimationCache::init(ImageCache& imageCache)
	{
		bool status = true;

		m_imageCache = &imageCache;

		DEBUG_INIT(AnimationCache);
		return status;
	}

	bool AnimationCache::shutdown()
	{
		bool status = true;

		DEBUG_SHUTDOWN(AnimationCache);
		return status;
	}

	uint32_t AnimationCache::getResourceID(const char* name)
	{
		for( auto id : m_allocated )
		{
			auto* ptr = m_data.get(id);
			if( ptr && ptr->m_name == name )
			{
				return id;
			}
		}
		return INVALID_ID;
	}

	const Animation* AnimationCache::getResource(uint32_t id)
	{
		return m_data.get(id);
	}

	LoadResult AnimationCache::process(const ResourceFile& file, LuaStack& lua, std::function<LoadResult(const AnimationDefaults&)> fn)
	{
		if( !lua.doFile(file) )
		{
			auto str = lua.to<std::string>();
			lua.pop();
			return{LoadResultFlag::Fail, str};
		}

		AnimationDefaults defaults;
		std::string texture = "";
		lua.pull("defaults");
		if( lua.is<luaTable>() )
		{
			defaults.loops = get(lua, "animationLoop", false);
			defaults.duration = get(lua, "animationDuration", 0.0f);
		}
		lua.pop();

		LoadResult res{LoadResultFlag::Success};
		for( lua.pairs("animations"); lua.next(); lua.pop(1) )
		{
			if( lua.is<std::string>(-2) )
			{
				res = fn(defaults);
				if( !res )
				{
					lua.pop(3);
					break;
				}
			}
		}
		lua.pop(1); //.sheet file table
		m_allocated = m_data.getActiveIDs();
		return res;
	}
	
	LoadResult AnimationCache::load(const ResourceFile& file, LuaStack& lua)
	{
		return process(file, lua, [&](const AnimationDefaults& defaults) -> LoadResult
		{
			auto id = getResourceID(lua.to<std::string>(-2).c_str());
			if( id == INVALID_ID )
			{
				id = m_data.create();
				auto res = loadAnimation(*m_data.get(id), lua, file.getHash(), defaults, *m_imageCache);
				if( !res )
				{
					m_data.release(id);
					return res;
				}
			}
			else
			{
				DEBUG_INFO("Skipping image ", lua.to<std::string>(-2), ", already loaded");
			}
			return{LoadResultFlag::Success};
		});
	}

	LoadResult AnimationCache::reload(const ResourceFile& file, LuaStack& lua)
	{
		return process(file, lua, [&](const AnimationDefaults& defaults) -> LoadResult
		{
			auto new_id = m_data.create();
			auto res = loadAnimation(*m_data.get(new_id), lua, file.getHash(), defaults, *m_imageCache);
			if( !res )
			{
				m_data.release(new_id);
				return res;
			}

			auto id = getResourceID(lua.to<std::string>(-2).c_str());
			if( id != INVALID_ID )
			{
				m_data.swapData(id, new_id);
				unloadAnimation(*m_data.get(new_id));
				m_data.release(new_id);
			}
			return{LoadResultFlag::Success};
		});
	}

	LoadResult AnimationCache::unload(const ResourceFile& file)
	{
		for( auto id : m_allocated )
		{
			auto* ptr = m_data.get(id);
			if( ptr && ptr->m_fileHash == file.getHash() )
			{
				unloadAnimation(*m_data.get(id));
				m_data.release(id);
			}
		}
		m_allocated = m_data.getActiveIDs();
		return{LoadResultFlag::Success};
	}
}
