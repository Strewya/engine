//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Caches/FontCache.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Caches/FileLoader.h>
#include <Caches/TextureCache.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	FontLoader::FontLoader(FileLoader& fileLoader, ScriptingSystem& scriptSystem, TextureCache& textures)
		: m_fileLoader(&fileLoader), m_scriptSystem(&scriptSystem), m_textures(&textures)
	{}

	bool FontLoader::load(FontData& fonts, const std::string& filename, uint32_t fileID) const
	{
		auto filter = [&](const char* name) -> uint32_t
		{
			auto id = findResourceByName(fonts, name);
			if(id == INVALID_ID)
			{
				return fonts.create();
			}
			DEBUG_INFO("Font ", name, " already loaded, skipping...");
			return INVALID_ID;
		};
		DataFile file(*m_scriptSystem);
		return file.open(filename.c_str()) && processLoading(fonts, file, fileID, filter);
	}

	bool FontLoader::reload(FontData& fonts, const std::string& filename, uint32_t fileID) const
	{
		auto filter = [&](const char* name) -> uint32_t
		{
			auto id = findResourceByName(fonts, name);
			if(id == INVALID_ID)
			{
				id = fonts.create();
			}
			else
			{
				unloadOne(fonts, id);
			}
			return id;
		};
		DataFile file(*m_scriptSystem);
		return file.open(filename.c_str()) && processLoading(fonts, file, fileID, filter);
	}

	void FontLoader::unloadOne(FontData& fonts, uint32_t id) const
	{
		auto& font = fonts.get(id);

		memset(font.m_glyphs, 0, sizeof(Glyph)*MAX_GLYPHS);
		font.m_name.clear();
		font.m_size = 0;
		m_fileLoader->unloadFile(m_textures->getResource(font.m_textureID).m_name.c_str());
		font.m_textureID = INVALID_ID;
	}

	void FontLoader::unloadAll(FontData& fonts) const
	{
		auto filter = [](const Font& f) { return true; };

		for(auto id = fonts.getID(filter); id != INVALID_ID; id = fonts.getID(filter))
		{
			unloadOne(fonts, id);
		}
	}

	void FontLoader::unloadFile(FontData& fonts, uint32_t fileID) const
	{
		for(auto id = findResourceByFileID(fonts, fileID); id != INVALID_ID; id = findResourceByFileID(fonts, fileID))
		{
			unloadOne(fonts, id);
			fonts.remove(id);
		}
	}

	bool FontLoader::processLoading(FontData& fonts, DataFile& file, uint32_t fileID, const Filter& filter) const
	{
		bool success = false;
		
		auto id = filter(file.getString("name", "").c_str());
		if(id != INVALID_ID)
		{
			success = parseFont(fonts.get(id), file, fileID);
			if(!success)
			{
				fonts.remove(id);
			}
		}
		return success;
	}

	bool FontLoader::parseFont(Font& font, DataFile& file, uint32_t fileID) const
	{
		bool status = false;

		auto name = file.getString("name", "");
		auto texture = file.getString("texture", "");
		auto size = file.getInt("size", 0);

		if(!name.empty() && !texture.empty() && size > 0 && m_fileLoader->loadFile(texture.c_str()))
		{
			font.m_name = name;
			font.m_size = size;
			font.m_textureID = m_textures->getResourceID(texture.c_str());

			uint32_t glyphCount = file.getListSize("glyphs");
			if(file.getList("glyphs"))
			{
				auto counter = 0;
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
							font.m_glyphs[glyphIndex].m_ascii = ascii;
							font.m_glyphs[glyphIndex].m_character = static_cast<char>(font.m_glyphs[glyphIndex].m_ascii);
							font.m_glyphs[glyphIndex].m_left = left;
							font.m_glyphs[glyphIndex].m_right = right;
							font.m_glyphs[glyphIndex].m_top = top;
							++counter;
						}
						else
						{
							DEBUG_INFO("Glyph in ", file.getFilename(), " font sheet is invalid: ", ascii, ",", left, ",", right, ",", top);
						}
						file.popList();
					}
				}
				file.popList();
				status = (counter == glyphCount);
			}
		}
		return status;
	}
}
