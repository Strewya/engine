//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Caches/TextureCache.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Graphics/GraphicsSystem.h>
#include <Util/ResourceFile.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	bool TextureCache::init(GraphicsSystem& graphics)
	{
		bool status = true;

		m_graphics = &graphics;

		DEBUG_INIT(TextureCache);
		return status;
	}

	bool TextureCache::shutdown()
	{
		bool status = true;

		//for each texture pair, call release? or do i let the graphics system shutdown handle it?
		// what if i decide to destroy the texture manager, but not the graphics? i get a leak...
		// when that happens, implement it.

		DEBUG_SHUTDOWN(TextureCache);
		return status;
	}

	uint32_t TextureCache::getResourceID(const char* name)
	{
		for( auto id : m_allocated )
		{
			const auto* ptr = m_data.get(id);
			if( ptr && ptr->m_name == name )
			{
				return id;
			}
		}
		return INVALID_ID;
	}

	const Texture* TextureCache::getResource(uint32_t id)
	{
		return m_data.get(id);
	}

	LoadResult TextureCache::load(const ResourceFile& file)
	{
		auto id = getResourceID(file.getName().c_str());
		if( id == INVALID_ID )
		{
			id = m_data.create();
			auto* ptr = m_data.get(id);
			if( ptr )
			{
				m_graphics->loadTexture(file, *ptr);
			}
			else
			{
				m_data.release(id);
				return{LoadResultFlag::Fail, std::string("Failed to load texture ") + file.getName()};
			}
		}
		m_allocated = m_data.getActiveIDs();
		return{LoadResultFlag::Success};
	}

	LoadResult TextureCache::reload(const ResourceFile& file)
	{
		auto id = getResourceID(file.getName().c_str());
		if( id != INVALID_ID )
		{
			auto* ptr = m_data.get(id);
			if( ptr )
			{
				m_graphics->reloadTexture(file, *ptr);
			}
			else
			{
				return{LoadResultFlag::Fail, std::string("Failed to load texture ") + file.getName()};
			}
		}
		m_allocated = m_data.getActiveIDs();
		return{LoadResultFlag::Success};
	}

	LoadResult TextureCache::unload(const ResourceFile& file)
	{
		for( auto id : m_allocated )
		{
			auto* ptr = m_data.get(id);
			if( ptr && ptr->m_fileHash == file.getHash() )
			{
				m_graphics->unloadTexture(*ptr);
				m_data.release(id);
			}
		}
		m_allocated = m_data.getActiveIDs();
		return{LoadResultFlag::Success};
	}
}
