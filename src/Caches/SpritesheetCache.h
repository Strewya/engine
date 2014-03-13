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
	class DataFile;
	class TextureCache;

	class SpritesheetCache
	{
	public:
		bool init(TextureCache& textures);
		bool shutdown();

		bool loadSpritesheet(DataFile& df);


	private:
		TextureCache* m_textures;
		std::vector<Spritesheet> m_sheets;
	};
}
