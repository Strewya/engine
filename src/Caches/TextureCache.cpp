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

	uint32_t TextureCache::getTextureID(const char* name) const
	{
		using std::begin; using std::end;
		auto it = std::find_if(begin(m_loadedTextures), end(m_loadedTextures), [&](const Texture& tex)
		{
			return name == tex.m_name;
		});
		if(it != end(m_loadedTextures))
		{
			return std::distance(m_loadedTextures.begin(), it) + 1;
		}
		return 0;
	}

	const Texture& TextureCache::getTexture(uint32_t id) const
	{
		--id;
		assert(id < m_loadedTextures.size());
		return m_loadedTextures[id];
	}

	Vec2 TextureCache::getTextureDimensions(uint32_t texID) const
	{
		const auto& t = getTexture(texID);
		return m_graphics->getTextureDimensions(t.m_rawTextureID);
	}

	bool TextureCache::load(const ResourceFile& file)
	{
		using std::begin; using std::end;
		auto it = std::find_if(begin(m_loadedTextures), end(m_loadedTextures), [](const Texture& tex)
		{
			return tex.m_name.empty();
		});
		if(it == end(m_loadedTextures))
		{
			m_loadedTextures.emplace_back();
			m_graphics->loadTexture(file, m_loadedTextures.back());
			DEBUG_INFO("Loaded texture '", file, "'.");
			return true;
		}
		return false;
	}

	bool TextureCache::reload(const ResourceFile& file)
	{
		using std::begin; using std::end;
		auto it = std::find_if(begin(m_loadedTextures), end(m_loadedTextures), [&](const Texture& tex)
		{
			return tex.m_name == file.getName();
		});
		if(it != end(m_loadedTextures))
		{
			m_graphics->reloadTexture(file, *it);
			DEBUG_INFO("Reloaded texture '", file, "'.");
			return true;
		}
		return false;
	}

	bool TextureCache::unload(const ResourceFile& file)
	{
		using std::begin; using std::end;
		auto it = std::find_if(begin(m_loadedTextures), end(m_loadedTextures), [&](const Texture& tex)
		{
			return tex.m_name == file.getName();
		});
		if(it != end(m_loadedTextures))
		{
			m_graphics->unloadTexture(*it);
			DEBUG_INFO("Unloaded texture '", file, "'.");
			return true;
		}
		return false;
	}
}
