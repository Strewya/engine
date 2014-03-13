#pragma once
/********************************************
*	class:	AnimationSystem
*	usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
/******* common headers *******/
/******* extra headers *******/
#include <Util/Time.h>
/******* end header inclusion *******/

namespace Core
{
	class AnimationCache;

	class AnimationData
	{
	public:
		Time m_timer;
		double m_timeScale;
		int32_t m_time;
		uint32_t m_animationID;
		uint32_t m_imageID;
	};

	class AnimationSystem
	{
	public:
		bool init(AnimationCache& animations);
		bool shutdown();

		void update(uint64_t dt);

		void registerData(AnimationData& data);
		void unregisterData(AnimationData& data);

	private:
		AnimationCache* m_animations;

		std::vector<AnimationData*> m_data;
	};
}
