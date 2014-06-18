#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include <Engine/Defines.h>
#include <deque>
	/*** extra headers if needed (alphabetically ordered) ***/
#include <Modules/Graphics/Service/Spritesheet.h>
#include <Util/AssetStorage.h>
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

namespace Graphics
{
	class SpritesheetCache : public Util::AssetStore<Spritesheet>
	{
	public:
		SpritesheetCache();
		void setTextureCache(ITextureCache& _textureCache);

		InstanceID loadSpritesheet(const char* name, Spritesheet** outPtr);
		Spritesheet* getSpritesheet(InstanceID handle);
		InstanceID getSpritesheet(const char* name, Spritesheet** outPtr);
		void clearSpritesheets();
		bool destroySpritesheet(InstanceID handle);

	protected:
		bool loadAsset(const char* filename, const LoadArgs* loadArgs, AssetPtr& asset);
		
	private:
		ITextureCache* _textureCache;
	};
}