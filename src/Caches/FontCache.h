#pragma once
/********************************************
*	class:	FontCache
*	usage:
********************************************/
/******* C++ headers *******/
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <DataStructs/Font.h>
/******* end header inclusion *******/

namespace Core
{
	class DataFile;
	class ResourceFile;
	class TextureCache;

	class FontCache
	{
	public:
		bool init(TextureCache& textures);
		bool shutdown();

		uint32_t getFontID(const char* name) const;
		const Font& getFont(uint32_t id) const;

		bool load(const ResourceFile& file, DataFile& dataFile);
		bool reload(const ResourceFile& file, DataFile& dataFile);
		bool unload(const ResourceFile& file);

	private:
		TextureCache* m_textures;
		std::vector<Font> m_fonts;
	};
}
