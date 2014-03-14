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
	class DataFile;
	class SpritesheetCache;

	class AnimationCache
	{
	public:
		bool init(SpritesheetCache& spritesheets);
		bool shutdown();

		bool loadAnimations(DataFile& df);

		uint32_t getAnimationID(const char* name) const;
		const Animation& getAnimation(uint32_t id) const;

	private:
		SpritesheetCache* m_spritesheets;

		std::vector<Animation> m_animations;
	};
}
