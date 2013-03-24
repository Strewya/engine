#ifndef SUBSYSTEMS_GRAPHICS_SPRITESHEETCACHE_H_
#define SUBSYSTEMS_GRAPHICS_SPRITESHEETCACHE_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
#include <deque>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Subsystems/Graphics/Spritesheet.h"
	/*** end header inclusion ***/

namespace Core
{
	class ResourceLocator;
	class ServiceLocator;
}
namespace Graphics
{
	class TextureCache;
}

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
		Spritesheet& LoadFromFile(const char* filename);
		Spritesheet& LoadFromFile(const String& filename);
		bool Exists(const char* sheetName);
		bool Exists(const String& sheetName);
		
		void setReferences(const Core::ResourceLocator& resources, const Core::ServiceLocator& services);
		
	private:
		std::deque<Spritesheet> _cache;
		std::deque<Spritesheet>::iterator _Find(const char* sheetName);
		TextureCache* _textureCache;

	};
}

#endif //SUBSYSTEMS_GRAPHICS_SPRITESHEETCACHE_H_