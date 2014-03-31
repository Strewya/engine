//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Caches/SpritesheetCache.h>
/******* C++ headers *******/
#include <cassert>
/******* extra headers *******/
#include <Caches/ImageCache.h>
#include <Caches/TextureCache.h>
#include <Util/ConfigFile.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	bool fillSheet(ConfigFile& df, Spritesheet& sheet, TextureCache& textures, ImageCache& images);

	bool SpritesheetCache::init(TextureCache& textures, ImageCache& images)
	{
		bool status = true;

		m_textures = &textures;
		m_images = &images;

		DEBUG_INIT(SpritesheetCache);
		return status;
	}

	bool SpritesheetCache::shutdown()
	{
		bool status = true;

		DEBUG_SHUTDOWN(SpritesheetCache);
		return status;
	}

	bool SpritesheetCache::loadSpritesheet(ConfigFile& file)
	{
		using std::begin; using std::end;
		bool status = false;
		auto name = file.getFilename();

		auto it = std::find_if(begin(m_sheets), end(m_sheets), [&](const Spritesheet& sheet)
		{
			return sheet.m_name == name;
		});

		if(it == end(m_sheets))
		{
			m_sheets.emplace_back();
			m_sheets.back().m_name = name;
			status = fillSheet(file, m_sheets.back(), *m_textures, *m_images);
		}
		else
		{
			DEBUG_INFO("Cannot init spritesheet ", name, ", already loaded!");
		}
		return status;
	}

	bool SpritesheetCache::reloadSpritesheet(ConfigFile& file)
	{
		using std::begin; using std::end;
		bool status = false;
		auto name = file.getFilename();

		auto it = std::find_if(begin(m_sheets), end(m_sheets), [&](const Spritesheet& sheet)
		{
			return sheet.m_name == name;
		});

		if(it != end(m_sheets))
		{
			it->m_images.clear();
			it->m_name = name;
			it->m_textureID = -1;
			status = fillSheet(file, *it, *m_textures, *m_images);
		}
		else
		{
			DEBUG_INFO("Cannot reload spritesheet ", name, ", not loaded!");
		}
		
		return status;
	}

	uint32_t SpritesheetCache::getSpritesheetID(const char* name) const
	{
		using std::begin; using std::end;
		auto it = std::find_if(begin(m_sheets), end(m_sheets), [&](const Spritesheet& sheet)
		{
			return name == sheet.m_name;
		});

		if(it != end(m_sheets))
		{
			return std::distance(begin(m_sheets), it);
		}
		return -1;
	}

	const Spritesheet& SpritesheetCache::getSpritesheet(uint32_t id) const
	{
		assert(id < m_sheets.size());
		return m_sheets[id];
	}

	bool fillSheet(ConfigFile& file, Spritesheet& sheet, TextureCache& textures, ImageCache& images)
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
							Image img;
							img.m_name.assign(imgName);

							img.m_ratio = w / h;
							Vec2 wh = pos + Vec2(w, h);

							img.m_texCoords[0].x = pos.x / tw;
							img.m_texCoords[0].y = pos.y / th;
							img.m_texCoords[1].x = wh.x / tw;
							img.m_texCoords[1].y = img.m_texCoords[0].y;
							img.m_texCoords[2].x = img.m_texCoords[0].x;
							img.m_texCoords[2].y = wh.y / th;
							img.m_texCoords[3].x = img.m_texCoords[1].x;
							img.m_texCoords[3].y = img.m_texCoords[2].y;

							if(!images.addImage(img, &sheet.m_images[i]))
							{
								DEBUG_INFO("Failed to add image with name ", imgName, ", skipping it!");
							}
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