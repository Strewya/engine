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
#include <DataStructs/Spritesheet.h>
/******* end header inclusion *******/

namespace Core
{
	class AnimationCache;
	class DataFile;
	class ImageCache;
	class TextureCache;
	

	class SpritesheetCache
	{
	public:
		bool init(AnimationCache& animations, ImageCache& images, TextureCache& textures);
		bool shutdown();

		bool loadFromFile(DataFile& file, bool reload);

		uint32_t getSpritesheetID(const char* name) const;
		const Spritesheet& getSpritesheet(uint32_t id) const;

	private:
		AnimationCache* m_animations;
		TextureCache* m_textures;
		ImageCache* m_images;
		std::vector<Spritesheet> m_sheets;
	};
}
