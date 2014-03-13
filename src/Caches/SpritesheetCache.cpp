//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Caches/SpritesheetCache.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Caches/TextureCache.h>
#include <Scripting/ScriptingSystem.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	void fillSheet(DataFile& df, Spritesheet& sheet, TextureCache& textures);

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

	bool SpritesheetCache::loadSpritesheet(DataFile& df)
	{
		std::string name = df.getString("name");
		using std::begin; using std::end;
		auto it = std::find_if(begin(m_sheets), end(m_sheets), [&](const Spritesheet& sheet)
		{
			return sheet.m_name == name;
		});
		if(it != end(m_sheets))
		{
			it->m_images.clear();
			it->m_name.clear();
			it->m_textureID = -1;
			fillSheet(df, *it, *m_textures);
		}
		else
		{
			m_sheets.emplace_back();
			fillSheet(df, m_sheets.back(), *m_textures);
		}
		return true;
	}

	void fillSheet(DataFile& file, Spritesheet& sheet, TextureCache& textures)
	{
		auto textureName = file.getString("texture");

		sheet.m_textureID = textures.getTextureID(textureName.c_str());

		if(sheet.m_textureID != -1)
		{
			Vec2 td = textures.getTextureDimensions(sheet.m_textureID);
			float tw = td.x;
			float th = td.y;

			uint32_t imgCount = file.getInt("imageCount");
			sheet.m_images.resize(imgCount);
			for(uint32_t i = 0; i < imgCount; ++i)
			{
				sheet.m_images[i].m_name = file.getString(("images[" + std::to_string(i + 1) + "].name").c_str());
				Vec2 pos = file.getVec2(("images[" + std::to_string(i + 1) + "].pos").c_str());
				float w = file.getFloat(("images[" + std::to_string(i + 1) + "].width").c_str());
				float h = file.getFloat(("images[" + std::to_string(i + 1) + "].height").c_str());
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
		}
	}


}
