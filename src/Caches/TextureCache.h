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
#include <DataStructs/Texture.h>
#include <Util/ObjectPool.h>
#include <Util/Vec2.h>
/******* end header inclusion *******/

namespace Core
{
	class GraphicsSystem;
	class ResourceFile;

	class TextureCache
	{
	public:
		bool init(GraphicsSystem& graphics);
		bool shutdown();

		uint32_t getTextureID(const char* path) const;
		const Texture& getTexture(uint32_t id) const;
		Vec2 getTextureDimensions(uint32_t texID) const;
		
		bool load(const ResourceFile& path);
		bool reload(const ResourceFile& path);
		bool unload(const ResourceFile& path);
		
	private:
		GraphicsSystem* m_graphics;

		std::vector<Texture> m_loadedTextures;
	};
}
