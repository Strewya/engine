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
	class ITextureCache;
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

		Spritesheet& getSpritesheet(uint32_t handle);

		uint32_t getSpritesheetHandle(const char* sheetName);
		uint32_t getSpritesheetHandle(const String& sheetName);
		
		uint32_t CreateEmpty(const char* sheetName);
		uint32_t CreateEmpty(const String& sheetName);

		uint32_t LoadFromFile(const char* filename);
		uint32_t LoadFromFile(const String& filename);
		
		bool Exists(uint32_t handle);
		
		void setReferences(const Core::ResourceLocator& resources, const Core::ServiceLocator& services);
		
	private:
		std::deque<Spritesheet> _cache;
		std::deque<Spritesheet>::iterator _Find(const char* sheetName);
		ITextureCache* _textureCache;
		Script::Engine* _script;

	};
}