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
		typedef ObjectContainer<Spritesheet> SpritesheetStore;

		SpritesheetLoader() = default;
		SpritesheetLoader(AnimationCache& animations, ImageCache& images);
		
		bool load(SpritesheetStore& spritesheets, DataFile& file) const;
		bool reload(SpritesheetStore& spritesheets, DataFile& file) const;
		bool unload(SpritesheetStore& spritesheets, uint32_t fileID) const;
		bool unloadAll(SpritesheetStore& spritesheets) const;

	private:
		AnimationCache* m_animations;
		ImageCache* m_images;
		ScriptingSystem* m_scriptSystem;
		
		bool parseSpritesheet(Spritesheet& spritesheets, DataFile& file, bool isReload, uint32_t* outID) const;
	};

	class SpritesheetCache : public ResourceCache < Spritesheet, SpritesheetLoader > {};
}
