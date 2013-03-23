#ifndef SUBSYSTEMS_GRAPHICS_SPRITESHEETCACHE_H_
#define SUBSYSTEMS_GRAPHICS_SPRITESHEETCACHE_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
#include <stack>
#include <vector>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Subsystems/Graphics/Spritesheet.h"
	/*** end header inclusion ***/

namespace Graphics
{
	class SpritesheetCache
	{
	public:
		SpritesheetCache();

		Spritesheet& getSpritesheet(const char* sheetName);
		Spritesheet& getSpritesheet(const String& sheetName);
		Spritesheet& CreateEmpty(const char* sheetName);
		Spritesheet& CreateEmpty(const String& sheetName);
		Spritesheet& CreateFromFile(const char* filename);
		Spritesheet& CreateFromFile(const String& filename);
		Spritesheet& Insert(const Spritesheet& sheet);
		bool Exists(const char* sheetName);
		bool Exists(const String& sheetName);
		void DestroyAll();
		void Destroy(const char* sheetName);
		void Destroy(const String& sheetName);
		
		
	private:
		struct DataWrapper
		{
			uint id;
			bool valid;
			Spritesheet sheet;
		};
		std::vector<DataWrapper> _cache;
		std::stack<uint> _freeSlots;
		std::vector<DataWrapper>::iterator Find(const char* sheetName);

	};
}

#endif //SUBSYSTEMS_GRAPHICS_SPRITESHEETCACHE_H_