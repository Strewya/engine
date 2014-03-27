//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Caches/AnimationCache.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Caches/SpritesheetCache.h>
#include <DataStructs/Spritesheet.h>
#include <Util/ConfigFile.h>
#include <Util/Time.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	static bool fillAnimation(ConfigFile& file, Animation& anim, const Spritesheet& spritesheets);

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

	bool AnimationCache::loadAnimations(ConfigFile& file)
	{
		return parseConfig(file, false);
	}

	bool AnimationCache::reloadAnimations(ConfigFile& file)
	{
		return parseConfig(file, true);
	}

	bool AnimationCache::parseConfig(ConfigFile& file, bool isReload)
	{
		bool success = false;
		std::string spritesheetName = file.getString("spritesheet", "");
		if(!spritesheetName.empty())
		{
			auto sheetID = m_spritesheets->getSpritesheetID(RESOURCE_S(spritesheetName));
			auto& sheet = m_spritesheets->getSpritesheet(sheetID);
			uint32_t animCount = file.getListSize("animations");
			uint32_t size = m_animations.size();
			success = true;
			for(uint32_t i = 0; i < animCount; ++i)
			{
				if(file.getListElement("animations", i + 1))
				{
					auto animName = file.getString("name", "");
					auto animID = getAnimationID(animName.c_str());
					if(isReload)
					{
						if(animID != -1)
						{
							//ok
							m_animations[animID].m_defaultDuration = 0;
							m_animations[animID].m_defaultRepeat = false;
							m_animations[animID].m_name.clear();
							m_animations[animID].m_sequence.clear();
							m_animations[animID].m_spritesheetID = sheetID;
							success &= fillAnimation(file, m_animations[animID], sheet);
						}
						else
						{
							DEBUG_INFO("Cannot reload animation ", animName, ", doesn't exist!");
						}
					}
					else
					{
						if(animID == -1)
						{
							//ok
							animID = m_animations.size();
							m_animations.emplace_back();
							m_animations[animID].m_spritesheetID = sheetID;
							success &= fillAnimation(file, m_animations[animID], sheet);
						}
						else
						{
							DEBUG_INFO("Cannot init animation ", animName, ", already exists!");
						}
					}
					file.popListElement();
				}
			}
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
	
	static bool fillAnimation(ConfigFile& file, Animation& anim, const Spritesheet& sheet)
	{	
		anim.m_name = file.getString("name", "");
		anim.m_defaultDuration = static_cast<uint32_t>(Time::secondsToMicros(file.getFloat("duration", 0)));
		anim.m_defaultRepeat = file.getBool("loop", false);
		uint32_t animImageCount = file.getListSize("images");
		anim.m_sequence.resize(animImageCount);
		for(uint32_t i = 0; i < animImageCount; ++i)
		{
			if(file.getListElement("images", i + 1))
			{
				uint32_t index = file.getInt(-1);
				if(index == -1)
				{
					index = 0;
					DEBUG_INFO("Image index in ", anim.m_name, " is invalid");
				}
				anim.m_sequence[i] = index;
				file.popListElement();
			}
		}
		return true;
	}
}
