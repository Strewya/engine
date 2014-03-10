//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Animation/AnimationSystem.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Graphics/GraphicsSystem.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	bool AnimationSystem::init(GraphicsSystem& graphics)
	{
		bool status = true;

		m_graphics = &graphics;

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
			auto& anim = m_graphics->getAnimation(animData.m_animationID);

			animData.m_timer.updateBy(dt, animData.m_timeScale);
			animData.m_time += animData.m_timer.getDeltaMicros();

			if(anim.m_isLooped)
			{
				wrap<int32_t>(0, anim.m_duration, animData.m_time);
			}
			else
			{
				clamp<int32_t>(0, anim.m_duration, animData.m_time);
			}

			float time = static_cast<float>(animData.m_time) / static_cast<float>(anim.m_duration);
			uint32_t animIndex = static_cast<uint32_t>(time*anim.m_images.size());
			if(animIndex == anim.m_images.size())
				--animIndex;

			animData.m_imageID = anim.m_images[animIndex];
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