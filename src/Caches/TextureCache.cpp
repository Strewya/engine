//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Caches/TextureCache.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Graphics/GraphicsSystem.h>
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

	uint32_t TextureCache::getTextureID(const char* path)
	{
		using std::begin; using std::end;
		auto it = std::find_if(begin(m_loadedTextures), end(m_loadedTextures), [&](const std::pair<std::string, uint32_t>& tex)
		{
			return path == tex.first;
		});
		uint32_t textureID = -1;
		if(it != end(m_loadedTextures))
		{
			textureID = it->second;
		}
		else
		{
			textureID = m_graphics->loadTextureFromFile(path);
			m_loadedTextures.emplace_back(path, textureID);
		}
		return textureID;
	}

	Vec2 TextureCache::getTextureDimensions(uint32_t texID) const
	{
		return m_graphics->getTextureDimensions(texID);
	}
}
