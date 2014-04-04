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
	class AnimationCache
	{
	public:
		bool init();
		bool shutdown();

		bool addAnimations(const Animation& anim, bool reload, uint32_t* outIndex = nullptr);

		uint32_t getAnimationID(const char* name) const;
		const Animation& getAnimation(uint32_t id) const;
		
	private:
		std::vector<Animation> m_animations;
	};
}
