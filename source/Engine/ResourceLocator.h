#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common header ***/
#include "Engine/Defines.h"
	/*** extra headers if needed ***/
	/*** end header inclusion ***/

namespace Graphics
{
	class SpritesheetCache;
	class TextureCache;
}

namespace Core
{
	class ResourceLocator
	{
	private:
		Graphics::TextureCache* _textureCache;
		Graphics::SpritesheetCache* _spritesheetCache;

	public:
		ResourceLocator();
		
		void Register(Graphics::TextureCache& cache);
		Graphics::TextureCache& getTextureCache() const;

		void Register(Graphics::SpritesheetCache& cache);
		Graphics::SpritesheetCache& getSpritesheetCache() const;
	};
}