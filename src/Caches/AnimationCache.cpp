//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Caches/AnimationCache.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Caches/ImageCache.h>
#include <Util/DataFile.h>
#include <Util/Time.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	AnimationLoader::AnimationLoader(ImageCache& images)
		: m_images(&images)
	{}

	bool AnimationLoader::load(AnimationData& animations, DataFile& file, uint32_t fileID) const
	{
		auto filter = [&](const char* name) -> uint32_t
		{
			auto id = findResourceByName(animations, name);
			if(id == INVALID_ID)
			{
				return animations.create();
			}
			DEBUG_INFO("Animation '", name, "' already loaded, skipping...");
			return INVALID_ID;
		};
		return processLoading(animations, file, fileID, filter);
	}

	bool AnimationLoader::reload(AnimationData& animations, DataFile& file, uint32_t fileID) const
	{
		auto filter = [&](const char* name)
		{
			auto id = findResourceByName(animations, name);
			if(id == INVALID_ID)
			{
				id = animations.create();
			}
			else
			{
				unloadOne(animations, id);
			}
			return id;
		};
		return processLoading(animations, file, fileID, filter);
	}

	void AnimationLoader::unloadOne(AnimationData& animations, uint32_t id) const
	{
		auto& anim = animations.get(id);
		anim.m_duration = 0;
		anim.m_name.clear();
		anim.m_repeats = false;
		anim.m_sequence.clear();
	}

	void AnimationLoader::unloadAll(AnimationData& animations) const
	{
		auto filter = [](const Animation& a) { return true; };

		for(auto id = animations.getID(filter); id != INVALID_ID; id = animations.getID(filter))
		{
			unloadOne(animations, id);
		}
	}

	void AnimationLoader::unloadFile(AnimationData& animations, uint32_t fileID) const
	{
		for(auto id = findResourceByFileID(animations, fileID); id != INVALID_ID; id = findResourceByFileID(animations, fileID))
		{
			unloadOne(animations, id);
			animations.remove(id);
		}
	}

	bool AnimationLoader::processLoading(AnimationData& animations, DataFile& file, uint32_t fileID, const Filter& filter) const
	{
		bool status = false;
		auto defaultDuration = file.getFloat("defaultDuration", 0);
		auto defaultRepeat = file.getBool("defaultRepeat", false);
		auto animCount = file.getListSize("list");
		if(file.getList("list"))
		{
			status = true;
			for(uint32_t i = 0; i < animCount; ++i)
			{
				if(file.getList(i + 1))
				{
					auto animID = filter(file.getString("name", "").c_str());
					if(animID != INVALID_ID)
					{
						auto success = parseAnimation(animations.get(animID), file, fileID, defaultDuration, defaultRepeat);
						if(!success)
						{
							animations.remove(animID);
						}
						status = status && success;
					}
					file.popList();
				}
				else
				{
					DEBUG_INFO("Element at index '", i + 1, "' is either nil or not a list!");
				}
			}
			file.popList();
		}
		else
		{
			DEBUG_INFO("The animation data contains no animation list!");
		}
		return status;
	}

	bool AnimationLoader::parseAnimation(Animation& anim, DataFile& file, uint32_t fileID, float defaultDuration, bool defaultRepeat) const
	{
		bool status = false;
		anim.m_name.assign(file.getString("name", ""));
		anim.m_fileID = fileID;
		anim.m_duration = Time::secondsToMicros(file.getFloat("duration", defaultDuration));
		anim.m_repeats = file.getBool("loop", defaultRepeat);
		auto imageCnt = file.getListSize("images");
		if(!anim.m_name.empty() && anim.m_duration > 0 && imageCnt > 0)
		{
			status = true;
			//anim valid, parse images
			anim.m_sequence.resize(imageCnt);
			if(file.getList("images"))
			{
				for(uint32_t i = 0; i < imageCnt; ++i)
				{
					auto imgName = file.getString(i + 1, "");
					auto imgID = m_images->getResourceID(imgName.c_str());
					if(imgID != INVALID_ID)
					{
						anim.m_sequence[i] = imgID;
					}
					else
					{
						DEBUG_INFO("Invalid image name ", imgName, " for animation ", anim.m_name);
						status = false;
					}
				}
				file.popList();
			}
			else
			{
				DEBUG_INFO("Could not get image list, either not exists of not a list!");
				status = false;
			}
		}
		else
		{
			DEBUG_INFO("Animation not valid!");
		}
		return status;
	}
}
