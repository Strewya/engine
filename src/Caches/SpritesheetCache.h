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

	class SpritesheetCache
	{
	public:
		bool loadSpritesheet(DataFile& df);


	private:
		
		std::vector<Spritesheet> m_sheets;
	};
}
