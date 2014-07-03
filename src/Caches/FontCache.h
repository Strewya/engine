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
	class ScriptingSystem;
	class TextureCache;

	class FontLoader
	{
	public:
		typedef ObjectContainer<Font> FontData;
		typedef std::function<uint32_t(const char* name)> Filter;

		FontLoader() = default;
		FontLoader(FileLoader& fileLoader, ScriptingSystem& scriptSystem, TextureCache& textures);

		bool load(FontData& fonts, const std::string& filename, uint32_t fileID) const;
		bool reload(FontData& fonts, const std::string& filename, uint32_t fileID) const;
		void unloadOne(FontData& fonts, uint32_t id) const;
		void unloadAll(FontData& fonts) const;
		void unloadFile(FontData& fonts, uint32_t fileID) const;

	private:
		FileLoader* m_fileLoader;
		ScriptingSystem* m_scriptSystem;
		TextureCache* m_textures;

		bool processLoading(FontData& fonts, DataFile& file, uint32_t fileID, const Filter& filter) const;
		bool parseFont(Font& font, DataFile& file, uint32_t fileID) const;
	};

	class FontCache : public ResourceCache < Font, FontLoader > {};
}
