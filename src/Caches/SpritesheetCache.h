#pragma once
/********************************************
*	class:	SpritesheetCache
*	usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <Caches/ResourceCache.h>
#include <DataStructs/Spritesheet.h>
/******* end header inclusion *******/

namespace Core
{
	class AnimationCache;
	class DataFile;
	class ImageCache;
	

	class SpritesheetLoader
	{
	public:
		typedef ObjectContainer<Spritesheet> SpritesheetData;
		typedef std::function<uint32_t(const char*)> Filter;

		SpritesheetLoader() = default;
		SpritesheetLoader(AnimationCache& animations, FileLoader& fileLoader, ImageCache& images, ScriptingSystem& scriptSystem);
		
		bool load(SpritesheetData& spritesheets, const std::string& filename, uint32_t fileID) const;
		bool reload(SpritesheetData& spritesheets, const std::string& filename, uint32_t fileID) const;
		void unloadFile(SpritesheetData& spritesheets, uint32_t fileID) const;
		void unloadAll(SpritesheetData& spritesheets) const;
		void unloadOne(SpritesheetData& spritesheets, uint32_t id) const;

	private:
		AnimationCache* m_animations;
		FileLoader* m_fileLoader;
		ImageCache* m_images;
		ScriptingSystem* m_scriptSystem;
		
		bool processLoading(SpritesheetData& spritesheets, DataFile& file, uint32_t fileID, const Filter& filter) const;
		bool parseSpritesheet(Spritesheet& spritesheets, DataFile& file, uint32_t fileID) const;
	};

	class SpritesheetCache : public ResourceCache < Spritesheet, SpritesheetLoader > {};
}
