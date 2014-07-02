//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Caches/SpritesheetCache.h>
/******* C++ headers *******/
#include <cassert>
/******* extra headers *******/
#include <Caches/AnimationCache.h>
#include <Caches/ImageCache.h>
#include <Caches/TextureCache.h>
#include <Util/DataFile.h>
#include <Util/Time.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	SpritesheetLoader::SpritesheetLoader(AnimationCache& animations, ImageCache& images)
		: m_animations(&animations), 
		  m_images(&images)
	{}

	bool SpritesheetLoader::load(SpritesheetVector& spritesheets, uint32_t* outID, DataFile& file) const
	{
		auto sheetName = file.getFilename();
		auto id = findResourceByName(spritesheets, sheetName.c_str());
			
		if(id == INVALID_ID)
		{
			if(outID != nullptr)
			{
				*outID = spritesheets.size();
			}
			spritesheets.emplace_back();
			return parseSpritesheet(spritesheets.back(), file, false, outID);
		}

		DEBUG_INFO("Spritesheet couldn't be loaded, name '", sheetName, "' already exists!");
		return false;
	}

	bool SpritesheetLoader::reload(SpritesheetVector& spritesheets, uint32_t* outID, DataFile& file) const
	{
		auto sheetName = file.getFilename();
		auto id = findResourceByName(spritesheets, sheetName.c_str());

		if(id != INVALID_ID)
		{
			if(outID != nullptr)
			{
				*outID = id;
			}
			return parseSpritesheet(spritesheets[id], file, true, outID);
		}

		DEBUG_INFO("Spritesheet couldn't be reloaded, name '", sheetName, "' doesn't exists!");
		return false;
	}

	bool SpritesheetLoader::unload(SpritesheetVector& spritesheets, uint32_t id) const
	{
		Spritesheet& ss = spritesheets[id];
		for(auto anim : ss.m_animations)
		{
			m_animations->destroyResource(anim);
		}
		for(auto img : ss.m_images)
		{
			m_images->destroyResource(img);
		}
		return true;
	}

	bool SpritesheetLoader::unloadAll(SpritesheetVector& spritesheets) const
	{
		while(!spritesheets.empty())
		{
			unload(spritesheets, 0);
		}
		return true;
	}

	bool SpritesheetLoader::parseSpritesheet(Spritesheet& sheet, DataFile& file, bool isReload, uint32_t* outID) const
	{
		assert(m_animations != nullptr && m_images != nullptr);

		bool success = false;
		sheet.m_name.assign(file.getFilename());
		if(file.getList("imageData"))
		{
			success = m_images->loadFromFile(isReload, &sheet.m_images, file);
			file.popList();
		}
		else
		{
			DEBUG_INFO("Config file '", file.getFilename(), "' contains no images!");
		}

		if(file.getList("animationData"))
		{
			success &= m_animations->loadFromFile(isReload, &sheet.m_animations, file);
			file.popList();
		}
		else
		{
			DEBUG_INFO("Config file '", file.getFilename(), "' contains no animations.");
		}

		return success;
	}
}