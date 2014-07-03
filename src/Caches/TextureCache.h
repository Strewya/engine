#pragma once
/********************************************
*	class:	TextureCache
*	usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <string>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <Caches/ResourceCache.h>
#include <DataStructs/Texture.h>
#include <Util/Vec2.h>
/******* end header inclusion *******/

namespace Core
{
	class GraphicsSystem;

	class TextureLoader
	{
	public:
		typedef ObjectContainer<Texture> TextureData;
		
		TextureLoader() = default;
		TextureLoader(GraphicsSystem& graphics);

		bool load(TextureData& textures, const std::string& texturePath, uint32_t fileID) const;
		bool reload(TextureData& textures, const std::string& texturePath, uint32_t fileID) const;
		void unloadOne(TextureData& textures, uint32_t resourceID) const;
		void unloadFile(TextureData& textures, uint32_t fileID) const;
		void unloadAll(TextureData& textures) const;

	private:
		friend class TextureCache;
		GraphicsSystem* m_graphics;

		bool processLoading(Texture& tex, const std::string& texturePath, uint32_t fileID) const;
	};



	class TextureCache : public ResourceCache<Texture, TextureLoader>
	{
	public:
		Vec2 getTextureDimensions(uint32_t texID) const;
	};
}
