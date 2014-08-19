//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Caches/ImageCache.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Caches/TextureCache.h>
#include <Scripting/LuaStack.h>
#include <Util/ResourceFile.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	bool ImageCache::init(TextureCache& textures)
	{
		bool status = true;

		m_textures = &textures;

		DEBUG_INIT(ImageCache);
		return status;
	}

	bool ImageCache::shutdown()
	{
		bool status = true;

		DEBUG_SHUTDOWN(ImageCache);
		return status;
	}

	uint32_t ImageCache::getResourceID(const char* name)
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

	const Image* ImageCache::getResource(uint32_t id)
	{
		return m_data.get(id);
	}

	LoadResult ImageCache::process(const ResourceFile& file, LuaStack& lua, std::function<LoadResult(const ImageDefaults&)> fn)
	{
		if( !lua.doFile(file) )
		{
			auto str = lua.to<std::string>();
			lua.pop();
			return{LoadResultFlag::Fail, str};
		}

		ImageDefaults defaults;
		std::string texture = "";
		lua.pull("defaults");
		if( lua.is<luaTable>() )
		{
			defaults.height = get(lua, "imageHeight", 0);
			defaults.width = get(lua, "imageWidth", 0);
			texture = get(lua, "imageTexture", std::string());
			defaults.textureID = m_textures->getResourceID(texture.c_str());
		}
		lua.pop();

		if( defaults.textureID == INVALID_ID )
		{
			return{LoadResultFlag::DependencyMissing, texture};
		}

		LoadResult res{LoadResultFlag::Success};
		for( lua.pairs("images"); lua.next(); lua.pop(1) )
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

	LoadResult ImageCache::load(const ResourceFile& file, LuaStack& lua)
	{
		return process(file, lua, [&](const ImageDefaults& defaults) -> LoadResult
		{
			auto id = getResourceID(lua.to<std::string>(-2).c_str());
			if( id == INVALID_ID )
			{
				id = m_data.create();
				auto res = loadImage(*m_data.get(id), lua, file.getHash(), defaults, *m_textures);
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

	LoadResult ImageCache::reload(const ResourceFile& file, LuaStack& lua)
	{
		return process(file, lua, [&](const ImageDefaults& defaults) -> LoadResult
		{
			auto new_id = m_data.create();
			auto res = loadImage(*m_data.get(new_id), lua, file.getHash(), defaults, *m_textures);
			if( !res )
			{
				m_data.release(new_id);
				return res;
			}

			auto id = getResourceID(lua.to<std::string>(-2).c_str());
			if( id != INVALID_ID )
			{
				m_data.swapData(id, new_id);
				unloadImage(*m_data.get(new_id));
				m_data.release(new_id);
			}
			return{LoadResultFlag::Success};
		});
	}

	LoadResult ImageCache::unload(const ResourceFile& file)
	{
		for( auto id : m_allocated )
		{
			auto* ptr = m_data.get(id);
			if( ptr && ptr->m_fileHash == file.getHash() )
			{
				unloadImage(*m_data.get(id));
				m_data.release(id);
			}
		}
		m_allocated = m_data.getActiveIDs();
		return{LoadResultFlag::Success};
	}
}
