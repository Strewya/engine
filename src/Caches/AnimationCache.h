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

	class AnimationCache
	{
	public:
		bool init(SpritesheetCache& spritesheets);
		bool shutdown();

		bool loadAnimations(ConfigFile& file);
		bool reloadAnimations(ConfigFile& file);

		uint32_t getAnimationID(const char* name) const;
		const Animation& getAnimation(uint32_t id) const;
		
	private:
		bool parseConfig(ConfigFile& file, bool isReload);
		SpritesheetCache* m_spritesheets;

		std::vector<Animation> m_animations;
	};
}
