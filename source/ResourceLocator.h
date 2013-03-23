#ifndef CORE_RESOURCELOCATOR_H_
#define CORE_RESOURCELOCATOR_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common header ***/
#include "Defines.h"
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

#endif //CORE_RESOURCELOCATOR_H_