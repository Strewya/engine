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
	class Animation;
	class AnimationCache;

	
	class AnimationPlayer
	{
	public:
		enum State
		{
			UNUSED,
			STOPPED,
			RUNNING
		};

		uint64_t m_startTime;
		uint64_t m_endTime;
		uint32_t* m_outImageID;
		uint32_t m_animationID;
		uint32_t m_id;
		State m_state;
		float m_playbackRate;
	};

	class AnimationSystem
	{
	public:
		bool init(AnimationCache& animations);
		bool shutdown();

		void update(uint64_t dt);

		uint32_t createPlayer(uint32_t& outImageID);
		void releasePlayer(uint32_t playerID);

		void startAnimation(uint32_t playerID, uint32_t animationID, float playbackRate);
		void stopAnimation(uint32_t playerID);
		bool isActive(uint32_t playerID);
		bool isRunning(uint32_t playerID);
		
	private:
		Time m_timer;
		std::vector<AnimationPlayer> m_runningAnimations;
		AnimationCache* m_animations;
		uint32_t m_idCounter;
		
		uint32_t findPlayer(uint32_t playerID);
		AnimationPlayer& getFreePlayer();
	};

	template<typename T, typename F>
	inline T interpolate(T start, T end, F fn, float time)
	{
		return start + static_cast<T>(static_cast<float>(end - start) * fn(time));
	}
}
