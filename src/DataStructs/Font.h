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
/******* end header inclusion *******/

namespace Core
{
	class DataFile;
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
		Glyph m_glyphs[MAX_GLYPHS];
		std::string m_name;
	};

	bool parseFont(Font& outFont, DataFile& file, TextureCache& textureCache);
}
