//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Caches/SpritesheetCache.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Caches/TextureCache.h>
#include <Util/ConfigFile.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	bool fillSheet(ConfigFile& df, Spritesheet& sheet, TextureCache& textures);

	bool SpritesheetCache::init(TextureCache& textures)
	{
		bool status = true;

		m_textures = &textures;

		DEBUG_INIT("SpritesheetCache");
		return status;
	}

	bool SpritesheetCache::shutdown()
	{
		bool status = true;

		DEBUG_SHUTDOWN("SpritesheetCache");
		return status;
	}

	bool SpritesheetCache::loadSpritesheet(ConfigFile& file)
	{
		using std::begin; using std::end;
		bool status = false;
		auto name = file.getString("name", "");
		if(!name.empty())
		{
			auto it = std::find_if(begin(m_sheets), end(m_sheets), [&](const Spritesheet& sheet)
			{
				return sheet.m_name == name;
			});

			if(it == end(m_sheets))
			{
				m_sheets.emplace_back();
				status = fillSheet(file, m_sheets.back(), *m_textures);
			}
			else
			{
				DEBUG_INFO("Cannot init-load spritesheet ", name, ", already loaded!");
			}
		}
		else
		{
			DEBUG_INFO("Config file error, 'name' doesn't exist in sheet ", file.getFilename());
		}
		return status;
	}

	bool SpritesheetCache::reloadSpritesheet(ConfigFile& file)
	{
		using std::begin; using std::end;
		bool status = false;
		auto name = file.getString("name", "");
		if(!name.empty())
		{
			auto it = std::find_if(begin(m_sheets), end(m_sheets), [&](const Spritesheet& sheet)
			{
				return sheet.m_name == name;
			});

			if(it != end(m_sheets))
			{
				it->m_images.clear();
				it->m_name.clear();
				it->m_textureID = -1;
				status = fillSheet(file, *it, *m_textures);
			}
			else
			{
				DEBUG_INFO("Cannot reload spritesheet ", name, ", not loaded!");
			}
		}
		else
		{
			DEBUG_INFO("Config file error, 'name' doesn't exist in sheet ", file.getFilename());
		}
		return status;
		
	}

	bool fillSheet(ConfigFile& file, Spritesheet& sheet, TextureCache& textures)
	{
		auto textureName = file.getString("texture", "");

		if(textureName.empty())
		{
			sheet.m_textureID = textures.getTextureID(textureName.c_str());

			if(sheet.m_textureID != -1)
			{
				Vec2 td = textures.getTextureDimensions(sheet.m_textureID);
				float tw = td.x;
				float th = td.y;

				if(file.getList("images"))
				{
					uint32_t imgCount = file.getListSize();
					sheet.m_images.resize(imgCount);
					for(uint32_t i = 0; i < imgCount; ++i)
					{
						file.getListElement(i);
						sheet.m_images[i].m_name = file.getString("name", "");
						Vec2 pos = file.getVec2("pos", Vec2(0, 0));
						float w = file.getFloat("width", 0);
						float h = file.getFloat("height", 0);

						if()
						sheet.m_images[i].m_ratio = w / h;
						Vec2 wh = pos + Vec2(w, h);

						sheet.m_images[i].m_texCoords[0].x = pos.x / tw;
						sheet.m_images[i].m_texCoords[0].y = pos.y / th;
						sheet.m_images[i].m_texCoords[1].x = wh.x / tw;
						sheet.m_images[i].m_texCoords[1].y = sheet.m_images[i].m_texCoords[0].y;
						sheet.m_images[i].m_texCoords[2].x = sheet.m_images[i].m_texCoords[0].x;
						sheet.m_images[i].m_texCoords[2].y = wh.y / th;
						sheet.m_images[i].m_texCoords[3].x = sheet.m_images[i].m_texCoords[1].x;
						sheet.m_images[i].m_texCoords[3].y = sheet.m_images[i].m_texCoords[2].y;
					}
					file.popList();
				}
			}
			else
			{
				DEBUG_INFO("The texture ", textureName, " doesn't exist!");
			}
		}
		else
		{
			DEBUG_INFO("Texture name is missing in file ", file.getFilename());
		}
	}


}
