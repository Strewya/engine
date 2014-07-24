//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <DataStructs/Font.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Caches/TextureCache.h>
#include <Util/DataFile.h>
/******* end headers *******/

namespace Core
{
	bool parseFont(Font& outFont, DataFile& file, TextureCache& textureCache)
	{
		auto name = file.getString("name", "");
		auto texture = file.getString("texture", "");
		auto size = file.getInt("size", 0);

		bool success = false;
		if(!name.empty() && !texture.empty() && size > 0)
		{
			outFont.m_name = name;
			outFont.m_size = size;
			outFont.m_textureID = textureCache.getTextureID(texture.c_str());
			success = outFont.m_textureID != 0;

			uint32_t glyphCount = file.getListSize("glyphs");
			if(success && file.getList("glyphs"))
			{
				uint32_t parsedGlyphsCounter = 0;
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
							outFont.m_glyphs[i].m_ascii = ascii;
							outFont.m_glyphs[i].m_character = static_cast<char>(outFont.m_glyphs[i].m_ascii);
							outFont.m_glyphs[i].m_left = left;
							outFont.m_glyphs[i].m_right = right;
							outFont.m_glyphs[i].m_top = top;

							++parsedGlyphsCounter;
						}
						else
						{
							DEBUG_INFO("Glyph in ", file.getFilename(), " font sheet is invalid: ", ascii, ",", left, ",", right, ",", top);
						}
						file.popList();
					}
				}
				file.popList();
				success = (parsedGlyphsCounter == MAX_GLYPHS);
			}
		}
		return success;
	}
}
