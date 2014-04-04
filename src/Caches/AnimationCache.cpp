//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Caches/AnimationCache.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Caches/ImageCache.h>
#include <Caches/SpritesheetCache.h>
#include <DataStructs/Spritesheet.h>
#include <Util/DataFile.h>
#include <Util/Time.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	bool AnimationCache::init()
	{
		bool status = true;

		DEBUG_INIT(AnimationCache);
		return status;
	}

	bool AnimationCache::shutdown()
	{
		bool status = true;

		DEBUG_SHUTDOWN(AnimationCache);
		return status;
	}

	bool AnimationCache::addAnimations(const Animation& anim, bool reload, uint32_t* outIndex)
	{
		bool success = false;
		uint32_t slot = -1;
		auto id = getAnimationID(anim.m_name.c_str());
		if(reload)
		{
			if(id == -1)
			{
				DEBUG_INFO("The animation '", anim.m_name, "' cannot be reloaded, it does not exist!");
			}
			else
			{
				slot = id;
			}
		}
		else
		{
			if(id != -1)
			{
				DEBUG_INFO("Cannot load animation '", anim.m_name, "', name already exists!");
			}
			else
			{
				slot = m_animations.size();
				m_animations.emplace_back();
			}
		}
		if(slot != -1)
		{
			m_animations[slot] = anim;
			if(outIndex != nullptr)
			{
				*outIndex = slot;
			}
			success = true;
		}
		return success;
	}
	
	uint32_t AnimationCache::getAnimationID(const char* name) const
	{
		using std::begin; using std::end;
		auto it = std::find_if(begin(m_animations), end(m_animations), [&](const Animation& anim)
		{
			return name == anim.m_name;
		});

		if(it != end(m_animations))
		{
			return std::distance(begin(m_animations), it);
		}
		return -1;
	}

	const Animation& AnimationCache::getAnimation(uint32_t id) const
	{
		assert(id < m_animations.size());
		return m_animations[id];
	}
}
