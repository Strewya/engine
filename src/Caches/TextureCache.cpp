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
	TextureLoader::TextureLoader(GraphicsSystem& graphics)
		: m_graphics(&graphics)
	{}

	bool TextureLoader::load(TextureVector& textures, uint32_t* outID, const std::string& texturePath) const
	{
		auto id = findResourceByName(textures, texturePath.c_str());
		if(id == INVALID_ID)
		{
			if(outID != nullptr)
			{
				*outID = textures.size();
			}
			textures.emplace_back();
			textures.back().m_name = texturePath;
			textures.back().m_index = m_graphics->loadTextureFromFile(texturePath.c_str());
			return textures.back().m_index != INVALID_ID;
		}

		if(outID != nullptr)
		{
			*outID = id;
		}
		DEBUG_INFO("Texture already loaded, skipping...");
		return true;
	}

	bool TextureLoader::reload(TextureVector& textures, uint32_t* outID, const std::string& texturePath) const
	{
		auto id = findResourceByName(textures, texturePath.c_str());
		if(id != INVALID_ID)
		{
			if(outID != nullptr)
			{
				*outID = id;
			}
			m_graphics->releaseTexture(textures[id].m_index);
			textures[id].m_name = texturePath;
			textures[id].m_index = m_graphics->loadTextureFromFile(texturePath.c_str());
			return textures[id].m_index != INVALID_ID;
		}

		DEBUG_INFO("Texture couldn't be reloaded, name '", texturePath, "' doesn't exist!");
		return false;
	}

	bool TextureLoader::unload(TextureVector& textures, uint32_t id)
	{
		m_graphics->releaseTexture(textures[id].m_index);
		return true;
	}

	bool TextureLoader::unloadAll(TextureVector& textures) const
	{
		//for each texture pair, call release? or do i let the graphics system shutdown handle it?
		// what if i decide to destroy the texture manager, but not the graphics? i get a leak...
		// when that happens, implement it.
		for(auto& tex : textures)
		{
			m_graphics->releaseTexture(tex.m_index);
		}
		return true;
	}

	bool TextureLoader::processLoading(Texture& tex, const std::string& texturePath) const
	{
		tex.m_name = texturePath;
	}
	

	Vec2 TextureCache::getTextureDimensions(uint32_t texID) const
	{
		return m_loader.m_graphics->getTextureDimensions(getResource(texID).m_index);
	}

	bool TextureCache::onFileModified(const std::string& path)
	{
		using std::begin; using std::end;
		auto it = std::find_if(begin(m_resources), end(m_resources), [&](const Texture& tex)
		{
			return ResourcePath(tex.m_name.c_str()) == path;
		});
		if(it != end(m_resources))
		{
			m_loader.m_graphics->releaseTexture(it->m_index);
			it->m_index = m_loader.m_graphics->loadTextureFromFile(it->m_name.c_str());
			return true;
		}
		return false;
	}
}
