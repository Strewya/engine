#pragma once
/********************************************
*	class:	ImageCache
*	usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <functional>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <DataStructs/Image.h>
#include <Loaders/Defines.h>
#include <Util/ObjectPool.h>
/******* end header inclusion *******/

namespace Core
{
	class LuaStack;
	class ResourceFile;
	class TextureCache;

	class ImageCache
	{
	public:
		bool init(TextureCache& textures);
		bool shutdown();

		uint32_t getResourceID(const char* name);
		const Image* getResource(uint32_t id);

		LoadResult load(const ResourceFile& file, LuaStack& lua);
		LoadResult reload(const ResourceFile& file, LuaStack& lua);
		LoadResult unload(const ResourceFile& file);

	private:
		TextureCache* m_textures;
		ObjectPool<Image> m_data;
		std::vector<uint32_t> m_allocated;

		LoadResult process(const ResourceFile& file, LuaStack& lua, std::function<LoadResult(const ImageDefaults&)> fn);
	};
}
