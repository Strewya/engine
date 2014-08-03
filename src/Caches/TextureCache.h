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
#include <Loaders/Defines.h>
#include <Util/ObjectPool.h>
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

		uint32_t getResourceID(const char* path);
		const Texture* getResource(uint32_t id);
		
		LoadResult load(const ResourceFile& path);
		LoadResult reload(const ResourceFile& path);
		LoadResult unload(const ResourceFile& path);
		
	private:
		GraphicsSystem* m_graphics;
		ObjectPool<Texture> m_data;
		std::vector<uint32_t> m_allocated;
	};
}
