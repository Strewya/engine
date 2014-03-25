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
		/* example sheet:
		{
			-- sheet name is its' filename
			texture = "Textures/tex.png",
			defaultWidth = 450, --optional, but each image MUST have both dimensions > 0
			defaultHeight = 300, --optional, see above
			images = {
				{ name = "frame01", pos = {0,0}, width = 450, height = 300},
				{ name = "frame02", pos = {450,300} },
				...
			}
		}

		*/
		bool success = false;

		auto textureName = file.getString("texture", "");
		if(!textureName.empty())
		{
			sheet.m_textureID = textures.getTextureID(textureName.c_str());

			if(sheet.m_textureID != -1)
			{
				Vec2 td = textures.getTextureDimensions(sheet.m_textureID);
				float tw = td.x;
				float th = td.y;
				auto defaultWidth = file.getInt("defaultWidth", 0);
				auto defaultHeight = file.getInt("defaultHeight", 0);
				auto imageCount = file.getListSize("images");
				sheet.m_images.resize(imageCount);
				for(uint32_t i = 0; i < imageCount; ++i)
				{
					if(file.getListElement("images", i + 1))
					{
						auto imgName = file.getString("name", "");
						auto imgWidth = file.getInt("width", defaultWidth);
						auto imgHeight = file.getInt("height", defaultHeight);
						auto pos = file.getVec2("pos", Vec2(-1, -1));
						if(!imgName.empty() && imgWidth > 0 && imgHeight > 0 && pos.x >= 0 && pos.y >= 0)
						{
							float w = static_cast<float>(imgWidth);
							float h = static_cast<float>(imgHeight);
							//image is valid, add it
							sheet.m_images[i].m_name.assign(imgName);

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
						else
						{
							DEBUG_INFO("Image in ", file.getFilename(), " spritesheet is invalid: ", imgName, ",", imgWidth, ",", imgHeight, ",", pos);
						}
						file.popListElement();
					}
				}
				success = !sheet.m_images.empty();
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
		return success;
	}
}