//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Caches/SpritesheetCache.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Scripting/ScriptingSystem.h>
/******* end headers *******/

namespace Core
{
	void fillSheet(DataFile& df, Spritesheet& sheet);

	bool SpritesheetCache::loadSpritesheet(DataFile& df)
	{}

	void fillSheet(DataFile& file, Spritesheet& sheet)
	{
		auto textureName = file.getString("texture");

		sheet.m_textureID = m_textureCache.getTextureID(textureName.c_str());

		if(m_sheetTextureID != -1)
		{
			ID3D11Resource* res = nullptr;
			m_textures[m_sheetTextureID]->GetResource(&res);

			ID3D11Texture2D* texture = nullptr;
			HRESULT hr = res->QueryInterface(&texture);

			if(SUCCEEDED(hr))
			{
				D3D11_TEXTURE2D_DESC desc;
				texture->GetDesc(&desc);
				float tw = static_cast<float>(desc.Width);
				float th = static_cast<float>(desc.Height);

				uint32_t imgCount = file.getInt("imageCount");
				uint32_t animCount = file.getInt("animationCount");
				m_sheet.m_images.resize(imgCount);
				m_sheet.m_animations.resize(animCount);
				for(uint32_t i = 0; i < imgCount; ++i)
				{
					m_sheet.m_images[i].m_name = file.getString(("images[" + std::to_string(i + 1) + "].name").c_str());
					Vec2 pos = file.getVec2(("images[" + std::to_string(i + 1) + "].pos").c_str());
					float w = file.getFloat(("images[" + std::to_string(i + 1) + "].width").c_str());
					float h = file.getFloat(("images[" + std::to_string(i + 1) + "].height").c_str());
					m_sheet.m_images[i].m_ratio = w / h;
					Vec2 wh = pos + Vec2(w, h);

					m_sheet.m_images[i].m_texCoords[0].x = pos.x / tw;
					m_sheet.m_images[i].m_texCoords[0].y = pos.y / th;
					m_sheet.m_images[i].m_texCoords[1].x = wh.x / tw;
					m_sheet.m_images[i].m_texCoords[1].y = m_sheet.m_images[i].m_texCoords[0].y;
					m_sheet.m_images[i].m_texCoords[2].x = m_sheet.m_images[i].m_texCoords[0].x;
					m_sheet.m_images[i].m_texCoords[2].y = wh.y / th;
					m_sheet.m_images[i].m_texCoords[3].x = m_sheet.m_images[i].m_texCoords[1].x;
					m_sheet.m_images[i].m_texCoords[3].y = m_sheet.m_images[i].m_texCoords[2].y;

				}

				for(uint32_t i = 0; i < animCount; ++i)
				{
					m_sheet.m_animations[i].m_name = file.getString(("animations[" + std::to_string(i + 1) + "].name").c_str());
					m_sheet.m_animations[i].m_duration = static_cast<uint32_t>(Time::secondsToMicros(file.getFloat(("animations[" + std::to_string(i + 1) + "].duration").c_str())));
					m_sheet.m_animations[i].m_isLooped = file.getString(("animations[" + std::to_string(i + 1) + "].type").c_str()) == "loop";
					uint32_t animImageCount = file.getInt(("animations[" + std::to_string(i + 1) + "].imageCount").c_str());
					m_sheet.m_animations[i].m_images.resize(animImageCount);
					for(uint32_t j = 0; j < animImageCount; ++j)
					{
						uint32_t index = file.getInt(("animations[" + std::to_string(i + 1) + "].images[" + std::to_string(j + 1) + "]").c_str());
						m_sheet.m_animations[i].m_images[j] = index;
					}
				}
			}
		}
		return m_sheetTextureID != -1;
	}


}
