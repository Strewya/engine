//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Caches/FontCache.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Caches/TextureCache.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	FontLoader::FontLoader(TextureCache& textures)
		: m_textures(&textures)
	{}

	bool FontLoader::load(FontVector& fonts, uint32_t* outID, DataFile& file) const
	{
		auto pd = [&](const char* name)
		{
			auto id = findResourceByName(fonts, name);
			if(id == INVALID_ID)
			{
				id = fonts.size();
				fonts.emplace_back();
				if(outID != nullptr)
				{
					*outID = id;
				}
				return id;
			}
			DEBUG_INFO("Resource with id ", name, " already loaded");
			return (uint32_t)INVALID_ID;
		};
		return processLoading(fonts, file, pd);
	}

	bool FontLoader::reload(FontVector& fonts, uint32_t* outID, DataFile& file) const
	{
		auto pd = [&](const char* name)
		{
			auto id = findResourceByName(fonts, name);
			if(id != INVALID_ID)
			{
				m_textures->destroyResource(fonts[id].m_textureID);
				fonts[id].m_textureID = INVALID_ID;
				memset(fonts[id].m_glyphs, 0, sizeof(Glyph)*MAX_GLYPHS);
				fonts[id].m_name.clear();
				fonts[id].m_size = 0;
				if(outID != nullptr)
				{
					*outID = id;
				}
				return id;
			}
			DEBUG_INFO("Resource with id ", name, " not loaded");
			return (uint32_t)INVALID_ID;
		};
		return processLoading(fonts, file, pd);
	}

	bool FontLoader::unload(FontVector& fonts, uint32_t id) const
	{
		m_textures->destroyResource(fonts[id].m_textureID);
		fonts.erase(fonts.begin() + id);
		return true;
	}

	bool FontLoader::unloadAll(FontVector& fonts) const
	{
		fonts.clear();
		return true;
	}

	bool FontLoader::processLoading(FontVector& fonts, DataFile& file, const Inserter& pd) const
	{
		bool success = false;
		
		auto name = file.getString("name", "");
		auto id = pd(name.c_str());
		if(id != INVALID_ID)
		{
			Font& font = fonts[id];
			auto texture = file.getString("texture", "");
			auto size = file.getInt("size", 0);

			if(!name.empty() && !texture.empty() && size > 0)
			{
				if(!m_font.m_texture.empty() && m_font.m_texture != texture)
				{
					//this means the texture for the font has changed, so we first release the existing one
					m_textureCache->destroyResource(m_fontTextureID);
				}
				m_font.m_name = name;
				m_font.m_texture = texture;
				m_font.m_size = size;

				success = m_textureCache->loadFromFile(false, &m_fontTextureID, m_font.m_texture);

				uint32_t glyphCount = file.getListSize("glyphs");
				if(success && file.getList("glyphs"))
				{
					for(uint32_t i = 0; i < glyphCount; ++i)
					{
						if(file.getList(i + 1))
						{
							auto ascii = file.getChar("char", 0);
							auto left = file.getInt("left", -1);
							auto right = file.getInt("right", -1);
							auto top = file.getInt("top", -1);
							if(ascii != 0 && left != -1 && right != -1 && top != -1)
							{
								auto glyphIndex = ascii - 32;
								m_font.m_glyphs[glyphIndex].m_ascii = ascii;
								m_font.m_glyphs[glyphIndex].m_character = static_cast<char>(m_font.m_glyphs[glyphIndex].m_ascii);
								m_font.m_glyphs[glyphIndex].m_left = left;
								m_font.m_glyphs[glyphIndex].m_right = right;
								m_font.m_glyphs[glyphIndex].m_top = top;
							}
							else
							{
								DEBUG_INFO("Glyph in ", file.getFilename(), " font sheet is invalid: ", ascii, ",", left, ",", right, ",", top);
							}
							file.popList();
						}
					}
					file.popList();
					success = true;
				}
			}
		}
		return success;
	}
}
