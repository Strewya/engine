//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Caches/FontCache.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Scripting/LuaStack.h>
#include <Util/ResourceFile.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	bool FontCache::init(TextureCache& textures)
	{
		bool status = true;

		m_textures = &textures;

		DEBUG_INIT(FontCache);
		return status;
	}

	bool FontCache::shutdown()
	{
		bool status = true;

		DEBUG_SHUTDOWN(FontCache);
		return status;
	}

	uint32_t FontCache::getResourceID(const char* name)
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

	const Font* FontCache::getResource(uint32_t id)
	{
		return m_data.get(id);
	}

	LoadResult FontCache::process(const ResourceFile& file, LuaStack& lua, std::function<LoadResult(void)> fn)
	{
		if( !lua.doFile(file) )
		{
			auto str = lua.to<std::string>();
			lua.pop();
			return{LoadResultFlag::Fail, str};
		}

		LoadResult res{LoadResultFlag::Success};
		for( lua.pairs(); lua.next(); lua.pop(1) )
		{
			if( lua.is<std::string>(-2) )
			{
				res = fn();
				if( !res )
				{
					lua.pop(2);
					break;
				}
			}
		}
		lua.pop();
		m_allocated = m_data.getActiveIDs();
		return res;
	}

	LoadResult FontCache::load(const ResourceFile& file, LuaStack& lua)
	{
		return process(file, lua, [&]() -> LoadResult
		{
			auto id = getResourceID(lua.to<std::string>(-2).c_str());
			if( id == INVALID_ID )
			{
				id = m_data.create();
				auto res = loadFont(*m_data.get(id), lua, file.getHash(), *m_textures);
				if( !res )
				{
					m_data.release(id);
					return res;
				}
			}
			return{LoadResultFlag::Success};
		});
	}

	LoadResult FontCache::reload(const ResourceFile& file, LuaStack& lua)
	{
		return process(file, lua, [&]() -> LoadResult
		{
			auto new_id = m_data.create();
			auto res = loadFont(*m_data.get(new_id), lua, file.getHash(), *m_textures);
			if( !res )
			{
				m_data.release(new_id);
				return res;
			}

			auto id = getResourceID(lua.to<std::string>(-2).c_str());
			if( id != INVALID_ID )
			{
				m_data.swapData(id, new_id);
				unloadFont(*m_data.get(new_id));
				m_data.release(new_id);
			}
			return{LoadResultFlag::Success};
		});
	}

	LoadResult FontCache::unload(const ResourceFile& file)
	{
		for( auto id : m_allocated )
		{
			auto* ptr = m_data.get(id);
			if( ptr && ptr->m_fileHash == file.getHash() )
			{
				unloadFont(*m_data.get(id));
				m_data.release(id);
			}
		}
		m_allocated = m_data.getActiveIDs();
		return{LoadResultFlag::Success};
	}
}
