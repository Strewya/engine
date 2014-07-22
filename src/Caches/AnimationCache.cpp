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

	bool AnimationLoader::load(AnimationVector& animations, std::vector<uint32_t>* outIDs, DataFile& file) const
	{
		auto inserter = [&](const Animation& anim)
		{
			auto id = findResourceByName(animations, anim.m_name.c_str());
			if(id == INVALID_ID)
			{
				if(outIDs != nullptr)
				{
					outIDs->emplace_back(animations.size());
				}
				animations.emplace_back(anim);
				return true;
			}
			DEBUG_INFO("Animation couldn't be loaded, name '", anim.m_name, "' already exists!");
			return false;
		};
		return processLoading(animations, file, inserter);
	}

	bool AnimationLoader::reload(AnimationVector& animations, std::vector<uint32_t>* outIDs, DataFile& file) const
	{
		auto inserter = [&](const Animation& anim)
		{
			auto id = findResourceByName(animations, anim.m_name.c_str());
			if(id == INVALID_ID)
			{
				animations.emplace_back(anim);
				//reloading, so not touching the outID
				return true;
			}
			DEBUG_INFO("Animation couldn't be reloaded, name '", anim.m_name, "' doesn't exists!");
			return false;
		};
		return processLoading(animations, file, inserter);
	}

	bool AnimationLoader::unload(AnimationVector& animations, uint32_t id) const
	{
		return true;
	}

	bool AnimationLoader::unloadAll(AnimationVector& animations) const
	{
		return true;
	}

	bool AnimationLoader::processLoading(AnimationVector& anims, DataFile& file, const Inserter& inserter) const
	{
		bool status = false;
		auto defaultDuration = file.getFloat("defaultDuration", 0);
		auto defaultRepeat = file.getBool("defaultRepeat", false);
		auto animCount = file.getListSize("list");
		anims.reserve(anims.size() + animCount);
		if(file.getList("list"))
		{
			status = true;
			for(uint32_t i = 0; i < animCount; ++i)
			{
				if(file.getList(i + 1))
				{
					Animation anim;
					if(parseAnimation(anim, file, defaultDuration, defaultRepeat))
					{
						status &= inserter(anim);
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

	bool AnimationLoader::parseAnimation(Animation& anim, DataFile& file, float defaultDuration, bool defaultRepeat) const
	{
		bool status = false;
		anim.m_name.assign(file.getString("name", ""));
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
