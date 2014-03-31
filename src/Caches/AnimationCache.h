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
/******* end header inclusion *******/

namespace Core
{
	class ConfigFile;
	class SpritesheetCache;
	class ImageCache;

	class AnimationCache
	{
	public:
		bool init(SpritesheetCache& spritesheets, ImageCache& images);
		bool shutdown();

		bool loadAnimations(ConfigFile& file, bool reload);

		uint32_t getAnimationID(const char* name) const;
		const Animation& getAnimation(uint32_t id) const;
		
	private:
		SpritesheetCache* m_spritesheets;
		ImageCache* m_images;

		std::vector<Animation> m_animations;
	};
}
