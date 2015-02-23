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
		DEBUG_STATUS(true);

		m_animations = &animations;
		m_idCounter = 0;

		DEBUG_INIT(AnimationSystem);
	}

	bool AnimationSystem::shutdown()
	{
		DEBUG_STATUS(true);

		DEBUG_SHUTDOWN(AnimationSystem);
	}

	void AnimationSystem::update(uint32_t deltaTime)
	{
		m_timer.updateBy(deltaTime);

		for(auto& player : m_runningAnimations)
		{
			if(player.m_state == AnimationPlayer::RUNNING)
			{
				const auto* animation = m_animations->getResource(player.m_animationID);
				uint32_t frameCount = animation->m_sequence.size();
				uint64_t current = m_timer.getCurrentMicros() - player.m_startTime;
				uint64_t length = player.m_endTime - player.m_startTime;
				float time = static_cast<float>(current) / static_cast<float>(length);
				auto startTime = player.m_startTime;
				auto endTime = player.m_endTime;
				auto playback = player.m_playbackRate;
				if(player.m_playbackRate < 0)
				{
					startTime = player.m_endTime;
					endTime = player.m_startTime;
					playback = -playback;
				}
				uint64_t referenceTime = interpolate(player.m_startTime, player.m_endTime, linear, time*playback);

				if(referenceTime >= player.m_endTime)
				{
					if(animation->m_loops)
					{
						player.m_startTime += animation->m_duration;
						player.m_endTime += animation->m_duration;
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
				*player.m_outImageID = animation->m_sequence[index];
			}
		}
	}

	uint32_t AnimationSystem::createPlayer(uint32_t& outImageID)
	{
		auto& player = getFreePlayer();
		player.m_outImageID = &outImageID;
		player.m_state = AnimationPlayer::STOPPED;
		DEBUG_INFO("Creating player ", player.m_id);
		return player.m_id;
	}

	void AnimationSystem::releasePlayer(uint32_t playerID)
	{
		auto index = findPlayer(playerID);
		if(index != -1)
		{
			m_runningAnimations[index].m_state = AnimationPlayer::UNUSED;
		}
	}

	void AnimationSystem::startAnimation(uint32_t playerID, uint32_t animationID, float playbackRate)
	{
		const auto* animation = m_animations->getResource(animationID);
		auto& player = m_runningAnimations[findPlayer(playerID)];
		player.m_animationID = animationID;
		player.m_startTime = m_timer.getCurrentMicros();
		player.m_endTime = player.m_startTime + animation->m_duration;
		player.m_playbackRate = playbackRate;
		player.m_state = AnimationPlayer::RUNNING;
	}

	void AnimationSystem::stopAnimation(uint32_t playerID)
	{
		auto index = findPlayer(playerID);
		if(index != -1)
		{
			m_runningAnimations[index].m_state = AnimationPlayer::STOPPED;
		}
	}

	bool AnimationSystem::isRunning(uint32_t playerID)
	{
		auto index = findPlayer(playerID);
		return index != -1 && m_runningAnimations[index].m_state == AnimationPlayer::RUNNING;
	}

	bool AnimationSystem::isActive(uint32_t playerID)
	{
		auto index = findPlayer(playerID);
		return index != -1 && m_runningAnimations[index].m_state != AnimationPlayer::UNUSED;
	}

	uint32_t AnimationSystem::findPlayer(uint32_t playerID)
	{
		using std::begin; using std::end;
		auto it = std::find_if(begin(m_runningAnimations), end(m_runningAnimations),
							   [=](const AnimationPlayer& p){ return p.m_id == playerID; });
		
      uint32_t index = UINT32_MAX;
		if(it != end(m_runningAnimations))
		{
			index = std::distance(begin(m_runningAnimations), it);
		}
		
		return index;
	}

	AnimationPlayer& AnimationSystem::getFreePlayer()
	{
		auto it = std::find_if(begin(m_runningAnimations), end(m_runningAnimations),
							   [](const AnimationPlayer& p){ return p.m_state == AnimationPlayer::UNUSED; });
		if(it == end(m_runningAnimations))
		{
			auto index = m_runningAnimations.size();
			m_runningAnimations.emplace_back();
			it = std::next(begin(m_runningAnimations), index);
		}
		
		it->m_id = ++m_idCounter;
		return *it;
	}
}
