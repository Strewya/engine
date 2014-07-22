#pragma once
/********************************************
*	class:	FontCache
*	usage:
********************************************/
/******* C++ headers *******/
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <Caches/ResourceCache.h>
#include <DataStructs/Font.h>
/******* end header inclusion *******/

namespace Core
{
	class DataFile;
	class TextureCache;

	class FontLoader
	{
	public:
		typedef std::vector<Font> FontVector;
		typedef std::function<uint32_t(const char* resourceName)> Inserter;

		FontLoader() = default;
		FontLoader(TextureCache& textures);

		bool load(FontVector& fonts, uint32_t* outID, DataFile& file) const;
		bool reload(FontVector& fonts, uint32_t* outID, DataFile& file) const;
		bool unload(FontVector& fonts, uint32_t id) const;
		bool unloadAll(FontVector& fonts) const;

	private:
		TextureCache* m_textures;

		bool processLoading(FontVector& fonts, DataFile& file, const Inserter& pd) const;
	};

	class FontCache : public ResourceCache < Font, FontLoader > {};
}
