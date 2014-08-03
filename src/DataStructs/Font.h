#pragma once
/********************************************
*	class:	Font
*	usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <string>
/******* common headers *******/
/******* extra headers *******/
#include <Loaders/Defines.h>
/******* end header inclusion *******/

namespace Core
{
	class LuaStack;
	class TextureCache;

	const uint32_t MAX_GLYPHS = 95;

	class Glyph
	{
	public:
		uint32_t m_ascii;
		uint32_t m_left;
		uint32_t m_right;
		uint32_t m_top;
		char m_character;
	};

	class Font
	{
	public:
		uint32_t m_size;
		uint32_t m_textureID;
		size_t m_fileHash;
		Glyph m_glyphs[MAX_GLYPHS];
		std::string m_name;
	};

	LoadResult loadFont(Font& outFont, LuaStack& file, size_t fileHash, TextureCache& textureCache);
	void unloadFont(Font& font);
}
