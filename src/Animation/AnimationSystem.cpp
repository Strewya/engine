//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Animation/AnimationSystem.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Caches/AnimationCache.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	static float linear(float t)
	{
		return t;
	}

	static float quadratic(float t)
	{
		return t*t;
	}

	typedef float(*Interpolator)(float);

	bool AnimationSystem::init(AnimationCache& animations)
	{
		bool status = true;

		m_animations = &animations;

		DEBUG_INIT(AnimationSystem);
		return status;
	}

	bool AnimationSystem::shutdown()
	{
		bool status = true;



		DEBUG_SHUTDOWN(AnimationSystem);
		return status;
	}

	void AnimationSystem::update(uint64_t deltaTime)
	{
		static Interpolator functions[] = {linear, quadratic};

		m_timer.updateBy(deltaTime);

		for(auto& player : m_runningAnimations)
		{
			if(player.m_state == AnimationPlayer::RUNNING)
			{
				const auto& animation = m_animations->getAnimation(player.m_animationID);
				uint32_t frameCount = animation.m_sequence.size();
				uint64_t current = m_timer.getCurMicros() - player.m_startTime;
				uint64_t length = player.m_endTime - player.m_startTime;
				float time = static_cast<float>(current) / static_cast<float>(length);
				uint64_t referenceTime = interpolate(player.m_startTime, player.m_endTime, functions[player.m_interpolation], time);

				if(referenceTime >= player.m_endTime)
				{
					if(player.m_loops)
					{
						player.m_startTime += animation.m_defaultDuration;
						player.m_endTime += animation.m_defaultDuration;
					}
					else
					{
						player.m_state = AnimationPlayer::STOPPED;
						continue;
					}
				}

				referenceTime -= player.m_startTime;
				uint64_t perFrame = length / static_cast<uint64_t>(frameCount);
				uint32_t index = 0;
				while(referenceTime > perFrame)
				{
					++index;
					referenceTime -= perFrame;
					if(index == frameCount - 1)
						break;
				}
				*player.m_outImageID = animation.m_sequence[index];
			}
		}
	}

	uint32_t AnimationSystem::startAnimation(uint32_t animationID, bool isLooped, float playbackRate, InterpolationType interpolationFn, uint32_t& outImageID)
	{
		const auto& animation = m_animations->getAnimation(animationID);
		auto& player = getFreePlayer();
		player.m_animationID = animationID;
		player.m_startTime = m_timer.getCurMicros();
		player.m_endTime = player.m_startTime + animation.m_defaultDuration;
		player.m_outImageID = &outImageID;
		player.m_playbackRate = playbackRate;
		player.m_loops = isLooped;
		player.m_state = AnimationPlayer::RUNNING;
		player.m_interpolation = interpolationFn;
		return player.m_id;
	}

	void AnimationSystem::stopAnimation(uint32_t playerID)
	{
		auto index = findPlayer(playerID);
		if(index != -1)
		{
			m_runningAnimations[index].m_state = AnimationPlayer::STOPPED;
		}
	}

	void AnimationSystem::pauseAnimation(uint32_t playerID)
	{
		auto index = findPlayer(playerID);
		if(checkState(index, AnimationPlayer::RUNNING))
		{
			m_runningAnimations[index].m_state = AnimationPlayer::PAUSED;
		}
	}

	void AnimationSystem::resumeAnimation(uint32_t playerID)
	{
		auto index = findPlayer(playerID);
		if(checkState(index, AnimationPlayer::PAUSED))
		{
			m_runningAnimations[index].m_state = AnimationPlayer::RUNNING;
		}
	}

	bool AnimationSystem::isStopped(uint32_t playerID)
	{
		auto index = findPlayer(playerID);
		return index == -1 || checkState(index, AnimationPlayer::STOPPED);
	}

	bool AnimationSystem::isPaused(uint32_t playerID)
	{
		auto index = findPlayer(playerID);
		return checkState(index, AnimationPlayer::PAUSED);
	}

	bool AnimationSystem::isRunning(uint32_t playerID)
	{
		auto index = findPlayer(playerID);
		return checkState(index, AnimationPlayer::RUNNING);
	}

	bool AnimationSystem::checkState(uint32_t index, AnimationPlayer::State state)
	{
		return index != -1 && m_runningAnimations[index].m_state == state;
	}

	uint32_t AnimationSystem::findPlayer(uint32_t playerID)
	{
		using std::begin; using std::end;
		for(auto it = begin(m_runningAnimations); it != end(m_runningAnimations); ++it)
		{
			if(it->m_id == playerID)
			{
				return std::distance(begin(m_runningAnimations), it);
			}
		}
		return -1;
	}

	AnimationPlayer& AnimationSystem::getFreePlayer()
	{
		for(auto& a : m_runningAnimations)
		{
			if(a.m_state == AnimationPlayer::STOPPED)
			{
				a.m_id = ++m_idCounter;
				return a;
			}
		}
		m_runningAnimations.emplace_back();
		m_runningAnimations.back().m_id = ++m_idCounter;
		return m_runningAnimations.back();
	}
}
