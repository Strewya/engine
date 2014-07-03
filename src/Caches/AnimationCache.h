#pragma once
/********************************************
*	class:	AnimationCache
*	usage:
********************************************/
/******* C++ headers *******/
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <Caches/ResourceCache.h>
#include <DataStructs/Animation.h>
/******* end header inclusion *******/

namespace Core
{
	class ImageCache;

	class AnimationLoader
	{
	public:
		typedef ObjectContainer<Animation> AnimationData;
		typedef std::function<uint32_t(const char* name)> Filter;

		AnimationLoader() = default;
		AnimationLoader(ImageCache& images);
		
		bool load(AnimationData& animations, DataFile& file, uint32_t fileID) const;
		bool reload(AnimationData& animations, DataFile& file, uint32_t fileID) const;
		void unloadOne(AnimationData& animations, uint32_t id) const;
		void unloadAll(AnimationData& animations) const;
		void unloadFile(AnimationData& animations, uint32_t fileID) const;

	private:
		ImageCache* m_images;

		bool processLoading(AnimationData& animations, DataFile& file, uint32_t fileID, const Filter& filter) const;
		bool parseAnimation(Animation& anim, DataFile& file, uint32_t fileID, float defaultDuration, bool defaultRepeat) const;
	};

	class AnimationCache : public ResourceCache < Animation, AnimationLoader > {};
}
