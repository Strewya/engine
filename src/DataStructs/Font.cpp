//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <DataStructs/Font.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Caches/TextureCache.h>
#include <Scripting/LuaStack.h>
/******* end headers *******/

namespace Core
{
	LoadResult loadFont(Font& outFont, LuaStack& lua, size_t fileHash, TextureCache& textureCache)
	{
		if( !lua.isString(-2) || !lua.isTable(-1) )
		{
			return {LoadResultFlag::Fail, "Invalid font format"};
		}

		outFont.m_name = lua.toString(-2);
		auto texture = getString(lua, "texture", "");
		outFont.m_textureID = textureCache.getResourceID(texture.c_str());
		if( outFont.m_textureID == INVALID_ID )
		{
			return {LoadResultFlag::DependencyMissing, texture};
		}

		outFont.m_size = getInt(lua, "size", 0);
		if( outFont.m_size == 0 )
		{
			return {LoadResultFlag::Fail, "Invalid font size"};
		}

		for( lua.ipairs("glyphs"); lua.next(); lua.pop(1) )
		{
			if( lua.isNumber(-2) && lua.isTable(-1) )
			{
				auto ascii = getChar(lua, "char", 0);
				auto left = getInt(lua, "left", -1);
				auto right = getInt(lua, "right", -1);
				auto top = getInt(lua, "top", -1);
				if( ascii != 0 && left != -1 && right != -1 && top != -1 )
				{
					auto i = ascii - 32;
					outFont.m_glyphs[i].m_ascii = ascii;
					outFont.m_glyphs[i].m_character = ascii;
					outFont.m_glyphs[i].m_left = left;
					outFont.m_glyphs[i].m_right = right;
					outFont.m_glyphs[i].m_top = top;
				}
			}
		}
		outFont.m_fileHash = fileHash;
		return {LoadResultFlag::Success};
	}

	void unloadFont(Font& font)
	{
		font.m_name.clear();
		font.m_size = 0;
		font.m_textureID = INVALID_ID;
		font.m_fileHash = 0;
		memset(font.m_glyphs, 0, sizeof(Glyph)*MAX_GLYPHS);
	}
}