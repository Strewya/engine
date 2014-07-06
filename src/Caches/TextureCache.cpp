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

	uint32_t TextureCache::getTextureID(const char* name)
	{
		using std::begin; using std::end;
		auto it = std::find_if(begin(m_loadedTextures), end(m_loadedTextures), [&](const Texture& tex)
		{
			return name == tex.m_name;
		});
		uint32_t textureID = -1;
		if(it != end(m_loadedTextures))
		{
			textureID = std::distance(m_loadedTextures.begin(), it);
		}
		else
		{
			textureID = m_graphics->loadTextureFromFile(name);
			m_loadedTextures.emplace_back(Texture{name, textureID});
		}
		return textureID;
	}

	Vec2 TextureCache::getTextureDimensions(uint32_t texID) const
	{
		return m_graphics->getTextureDimensions(texID);
	}

	bool TextureCache::releaseTexture(uint32_t texID)
	{
		if(texID < m_loadedTextures.size())
		{
			Texture& t = m_loadedTextures[texID];
			if(t.m_rawTextureID != -1)
			{
				m_graphics->releaseTexture(t.m_rawTextureID);
				t.m_name.clear();
				t.m_rawTextureID = -1;
				return true;
			}
		}
		return false;
	}

	bool TextureCache::onFileModified(const ResourceFile& file)
	{
		using std::begin; using std::end;
		auto it = std::find_if(begin(m_loadedTextures), end(m_loadedTextures), [&](const Texture& tex)
		{
			return tex.m_name == file.getName();
		});
		if(it != end(m_loadedTextures))
		{
			m_graphics->releaseTexture(it->m_rawTextureID);
			it->m_rawTextureID = m_graphics->loadTextureFromFile(it->m_name.c_str());
			DEBUG_INFO("Reloaded texture '", file, "'.");
			return true;
		}
		return false;
	}
}
