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
#include "Util/AssetStorage.h"
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
/*
namespace Script
{
	class Engine;
}
*/

namespace Graphics
{
	class SpritesheetCache : public Util::AssetStore<Spritesheet>
	{
	public:
		~SpritesheetCache();
		void setTextureCache(ITextureCache& _textureCache);

		InstanceID LoadSpritesheet(const char* name, Spritesheet** outPtr);
		Spritesheet* getSpritesheet(uint32_t handle);
		InstanceID getSpritesheet(const char* name, Spritesheet** outPtr);

	protected:
		bool Load(const char* filename, const LoadArgs* loadArgs, AssetPtr& asset);
		
	private:
		ITextureCache* _textureCache;
		//Script::Engine* _script;

	};
}