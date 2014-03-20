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
	class ConfigFile;
	class TextureCache;

	class SpritesheetCache
	{
	public:
		bool init(TextureCache& textures);
		bool shutdown();

		bool loadSpritesheet(ConfigFile& file);
		bool reloadSpritesheet(ConfigFile& file);

	private:
		TextureCache* m_textures;
		std::vector<Spritesheet> m_sheets;
	};
}
