#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
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
namespace Script
{
	class Engine;
}

namespace Graphics
{
	class SpritesheetCache
	{
	public:
		SpritesheetCache();

		Spritesheet& getSpritesheet(uint handle);

		uint getSpritesheetHandle(const char* sheetName);
		uint getSpritesheetHandle(const String& sheetName);
		
		uint CreateEmpty(const char* sheetName);
		uint CreateEmpty(const String& sheetName);

		uint LoadFromFile(const char* filename);
		uint LoadFromFile(const String& filename);
		
		bool Exists(uint handle);
		
		void setReferences(const Core::ResourceLocator& resources, const Core::ServiceLocator& services);
		
	private:
		std::deque<Spritesheet> _cache;
		std::deque<Spritesheet>::iterator _Find(const char* sheetName);
		TextureCache* _textureCache;
		Script::Engine* _script;

	};
}