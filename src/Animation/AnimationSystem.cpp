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
	bool AnimationSystem::init(AnimationCache& animations)
	{
		bool status = true;

		m_animations = &animations;

		DEBUG_INFO("GraphicsSystem init ", status ? "OK" : "FAIL");
		return status;
	}

	bool AnimationSystem::shutdown()
	{
		bool status = true;



		DEBUG_INFO("GraphicsSystem shutdown ", status ? "OK" : "FAIL");
		return status;
	}

	void AnimationSystem::update(uint64_t dt)
	{
		for(auto* ptr : m_data)
		{
			auto& animData = *ptr;
			const auto& anim = m_animations->getAnimation(animData.m_animationID);

			animData.m_timer.updateBy(dt);
			animData.m_time += animData.m_timer.getDeltaMicros();

			if(anim.m_defaultRepeat)
			{
				wrap<int32_t>(0, anim.m_defaultDuration, animData.m_time);
			}
			else
			{
				clamp<int32_t>(0, anim.m_defaultDuration, animData.m_time);
			}

			float time = static_cast<float>(animData.m_time) / static_cast<float>(anim.m_defaultDuration);
			uint32_t animIndex = static_cast<uint32_t>(time*anim.m_sequence.size());
			if(animIndex == anim.m_sequence.size())
				--animIndex;

			animData.m_imageID = anim.m_sequence[animIndex];
		}
	}

	void AnimationSystem::registerData(AnimationData& data)
	{
		m_data.emplace_back(&data);
	}

	void AnimationSystem::unregisterData(AnimationData& data)
	{
		for(auto it = std::begin(m_data); it != std::end(m_data); ++it)
		{
			if(&data == *it)
			{
				m_data.erase(it);
				return;
			}
		}
	}
}
