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
	SpritesheetLoader::SpritesheetLoader(AnimationCache& animations, FileLoader& fileLoader, ImageCache& images, ScriptingSystem& scriptSystem)
		: m_animations(&animations), m_fileLoader(&fileLoader), m_images(&images), m_scriptSystem(&scriptSystem)
	{}

	bool SpritesheetLoader::load(SpritesheetData& spritesheets, const std::string& filename, uint32_t fileID) const
	{
		auto filter = [&](const char* name) -> uint32_t
		{
			auto id = findResourceByName(spritesheets, name);
			if(id == INVALID_ID)
			{
				return spritesheets.create();
			}
			DEBUG_INFO("Spritesheet ", name, " already loaded, skipping...");
			return INVALID_ID;
		};
		DataFile file(*m_scriptSystem);
		return file.open(filename.c_str()) && processLoading(spritesheets, file, fileID, filter);
	}

	bool SpritesheetLoader::reload(SpritesheetData& spritesheets, const std::string& filename, uint32_t fileID) const
	{
		auto filter = [&](const char* name) -> uint32_t
		{
			auto id = findResourceByName(spritesheets, name);
			if(id == INVALID_ID)
			{
				id = spritesheets.create();
			}
			else
			{
				unloadOne(spritesheets, id);
			}
			return id;
		};
		DataFile file(*m_scriptSystem);
		return file.open(filename.c_str()) && processLoading(spritesheets, file, fileID, filter);
	}

	void SpritesheetLoader::unloadOne(SpritesheetData& spritesheets, uint32_t id) const
	{
		Spritesheet& sheet = spritesheets.get(id);
		sheet.m_name.clear();
		sheet.m_textureID
		for(auto anim : sheet.m_animations)
		{
			m_animations->destroyResource(anim);
		}
		for(auto img : sheet.m_images)
		{
			m_images->destroyResource(img);
		}
	}

	void SpritesheetLoader::unloadAll(SpritesheetData& spritesheets) const
	{
		auto filter = [](const Spritesheet& s) { return true; };

		for(auto id = spritesheets.getID(filter); id != INVALID_ID; id = spritesheets.getID(filter))
		{
			unloadOne(spritesheets, id);
		}
	}

	void SpritesheetLoader::unloadFile(SpritesheetData& spritesheets, uint32_t fileID) const
	{

	}

	bool SpritesheetLoader::processLoading(SpritesheetData& spritesheets, DataFile& file, uint32_t fileID, const Filter& filter)
	{

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