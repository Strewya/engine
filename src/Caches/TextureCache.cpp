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

	bool TextureLoader::load(TextureData& textures, const std::string& texturePath, uint32_t fileID) const
	{
		auto id = findResourceByName(textures, texturePath.c_str());
		if(id == INVALID_ID)
		{
			id = textures.create();
			bool success = processLoading(textures.get(id), texturePath, fileID);
			if(!success)
			{
				textures.remove(id);
				DEBUG_INFO("Failed to load texture ", texturePath);
			}
			return success;
		}
		DEBUG_INFO("Texture already loaded, skipping...");
		return true;
	}

	bool TextureLoader::reload(TextureData& textures, const std::string& texturePath, uint32_t fileID) const
	{
		auto id = findResourceByName(textures, texturePath.c_str());
		if(id != INVALID_ID)
		{
			unloadOne(textures, id);
		}
		else
		{
			id = textures.create();
		}
		return processLoading(textures.get(id), texturePath, fileID);
	}

	void TextureLoader::unloadOne(TextureData& textures, uint32_t id) const
	{
		auto& tex = textures.get(id);
		m_graphics->releaseTexture(tex.m_rawTextureID);
		tex.m_name.clear();
		tex.m_fileID = INVALID_ID;
		tex.m_rawTextureID = INVALID_ID;
	}

	void TextureLoader::unloadAll(TextureData& textures) const
	{
		auto anyFilter = [](const Texture& tex)
		{
			return true;
		};
		for(auto id = textures.getID(anyFilter); id != INVALID_ID; id = textures.getID(anyFilter))
		{
			unloadOne(textures, id);
		}
	}

	void TextureLoader::unloadFile(TextureData& textures, uint32_t fileID) const
	{
		for(auto id = findResourceByFileID(textures, fileID); id != INVALID_ID; id = findResourceByFileID(textures, fileID))
		{
			unloadOne(textures, id);
			textures.remove(id);
		}
	}

	bool TextureLoader::processLoading(Texture& tex, const std::string& texturePath, uint32_t fileID) const
	{
		tex.m_name = texturePath;
		tex.m_fileID = fileID;
		tex.m_rawTextureID = m_graphics->loadTextureFromFile(texturePath.c_str());
		return tex.m_rawTextureID != INVALID_ID;
	}
	

	Vec2 TextureCache::getTextureDimensions(uint32_t texID) const
	{
		return m_loader.m_graphics->getTextureDimensions(getResource(texID).m_rawTextureID);
	}
}
