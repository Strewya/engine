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
		typedef std::vector<Animation> AnimationVector;
		typedef std::function<bool(const Animation&)> Inserter;

		AnimationLoader() = default;
		AnimationLoader(ImageCache& images);
		
		bool load(AnimationVector& animations, std::vector<uint32_t>* outIDs, DataFile& file) const;
		bool reload(AnimationVector& animations, std::vector<uint32_t>* outIDs, DataFile& file) const;
		bool unload(AnimationVector& animations, uint32_t id) const;
		bool unloadAll(AnimationVector& animations) const;

	private:
		ImageCache* m_images;

		bool processLoading(AnimationVector& animations, DataFile& file, const Inserter& inserter) const;
		bool parseAnimation(Animation& anim, DataFile& file, float defaultDuration, bool defaultRepeat) const;
	};

	class AnimationCache : public ResourceCache < Animation, AnimationLoader > {};
}
