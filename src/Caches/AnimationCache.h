#pragma once
/********************************************
*	class:	AnimationCache
*	usage:
********************************************/
/******* C++ headers *******/
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <DataStructs/Animation.h>
#include <Loaders/Defines.h>
#include <Util/ObjectPool.h>
/******* end header inclusion *******/

namespace Core
{
	class ImageCache;
	class LuaStack;
	class ResourceFile;

	class AnimationCache
	{
	public:
		bool init(ImageCache& imageCache);
		bool shutdown();

		uint32_t getResourceID(const char* name);
		const Animation* getResource(uint32_t id);
		
		LoadResult load(const ResourceFile& file, LuaStack& lua);
		LoadResult reload(const ResourceFile& file, LuaStack& lua);
		LoadResult unload(const ResourceFile& file);

	private:
		ImageCache* m_imageCache;
		ObjectPool<Animation> m_data;
		std::vector<uint32_t> m_allocated;

		LoadResult process(const ResourceFile& file, LuaStack& lua, std::function<LoadResult(const AnimationDefaults&)> fn);
	};
}
