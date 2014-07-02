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
		typedef std::vector<Texture> TextureVector;
		
		TextureLoader() = default;
		TextureLoader(GraphicsSystem& graphics);

		bool load(TextureVector& textures, uint32_t* outID, const std::string& texturePath) const;
		bool reload(TextureVector& textures, uint32_t* outID, const std::string& texturePath) const;
		bool unload(TextureVector& textures, uint32_t ID);
		bool unloadAll(TextureVector& textures) const;

	private:
		friend class TextureCache;
		GraphicsSystem* m_graphics;

		bool processLoading(Texture& tex, const std::string& texturePath) const;
	};



	class TextureCache : public ResourceCache<Texture, TextureLoader>
	{
	public:
		Vec2 getTextureDimensions(uint32_t texID) const;
		
		bool onFileModified(const std::string& path);
	};
}
