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

	enum InterpolationType
	{
		IT_LINEAR,
		IT_QUADRATIC
	};
	
	class AnimationPlayer
	{
	public:
		enum State
		{
			STOPPED,
			PAUSED,
			RUNNING
		};

		uint64_t m_startTime;
		uint64_t m_endTime;
		uint32_t* m_outImageID;
		uint32_t m_animationID;
		uint32_t m_id;
		State m_state;
		float m_playbackRate;
		InterpolationType m_interpolation;
		bool m_loops;
	};

	class AnimationSystem
	{
	public:
		

		bool init(AnimationCache& animations);
		bool shutdown();

		void update(uint64_t dt);

		uint32_t startAnimation(uint32_t animationID, bool loop, float playbackRate, InterpolationType fn, uint32_t& outImageID);
		void stopAnimation(uint32_t playerID);
		void pauseAnimation(uint32_t playerID);
		void resumeAnimation(uint32_t playerID);
		bool isStopped(uint32_t playerID);
		bool isPaused(uint32_t playerID);
		bool isRunning(uint32_t playerID);
		
	private:
		Time m_timer;
		std::vector<AnimationPlayer> m_runningAnimations;
		AnimationCache* m_animations;
		uint32_t m_idCounter;
		
		bool checkState(uint32_t index, AnimationPlayer::State state);
		uint32_t findPlayer(uint32_t playerID);
		AnimationPlayer& getFreePlayer();
	};

	template<typename T, typename F>
	inline T interpolate(T start, T end, F fn, float time)
	{
		return start + static_cast<T>(static_cast<float>(end - start) * fn(time));
	}
}
