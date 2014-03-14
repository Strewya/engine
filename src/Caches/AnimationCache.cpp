//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Caches/AnimationCache.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Scripting/ScriptingSystem.h>
#include <Util/Time.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	void fillAnimation(DataFile& file, Animation& anim, SpritesheetCache& spritesheets);

	bool AnimationCache::init(SpritesheetCache& spritesheets)
	{
		bool status = true;

		m_spritesheets = &spritesheets;

		DEBUG_INIT("AnimationCache");
		return status;
	}

	bool AnimationCache::shutdown()
	{
		bool status = true;

		DEBUG_SHUTDOWN("AnimationCache");
		return status;
	}

	bool AnimationCache::loadAnimations(DataFile& file)
	{
		using std::begin; using std::end;

		uint32_t animCount = file.getArraySize("animations");
		for(uint32_t i = 0; i < animCount; ++i)
		{
			auto it = std::find_if(begin(m_animations), end(m_animations), [&](const Animation& anim)
			{
				return file.getFilename() == anim.m_name;
			});

			if(it != end(m_animations))
			{
				it->m_defaultDuration = 0;
				it->m_defaultRepeat = false;
				it->m_name.clear();
				it->m_sequence.clear();
				it->m_spritesheetID = -1;
				fillAnimation(file, *it, *m_spritesheets);
			}
			else
			{
				m_animations.emplace_back();
				fillAnimation(file, m_animations.back(), *m_spritesheets);
			}
		}
		return true; //when will i ever return false??? prolly when fill animation fails.. it should also return bool.
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

	void fillAnimation(DataFile& file, Animation& anim, SpritesheetCache& spritesheets)
	{
		
		
		{
			anim.m_name = file.getString(("animations[" + std::to_string(i + 1) + "].name").c_str());
			anim.m_defaultDuration = static_cast<uint32_t>(Time::secondsToMicros(file.getFloat(("animations[" + std::to_string(i + 1) + "].duration").c_str())));
			anim.m_defaultRepeat = file.getString(("animations[" + std::to_string(i + 1) + "].type").c_str()) == "loop";
			uint32_t animImageCount = file.getInt(("animations[" + std::to_string(i + 1) + "].imageCount").c_str());
			anim.m_sequence.resize(animImageCount);
			for(uint32_t j = 0; j < animImageCount; ++j)
			{
				uint32_t index = file.getInt(("animations[" + std::to_string(i + 1) + "].images[" + std::to_string(j + 1) + "]").c_str());
				anim.m_sequence[j] = index;
			}
		}
	}
}
