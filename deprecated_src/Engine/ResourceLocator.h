#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common header ***/
#include <Engine/Defines.h>
	/*** extra headers if needed ***/
	/*** end header inclusion ***/

namespace Graphics
{
	class SpritesheetCache;
	class ITextureCache;
}

namespace Core
{
	class ResourceLocator
	{
	private:
		Graphics::ITextureCache* _textureCache;
		Graphics::SpritesheetCache* _spritesheetCache;

	public:
		ResourceLocator();
		
		void Register(Graphics::ITextureCache& cache);
		Graphics::ITextureCache& getTextureCache() const;

		void Register(Graphics::SpritesheetCache& cache);
		Graphics::SpritesheetCache& getSpritesheetCache() const;
	};
}