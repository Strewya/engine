#ifndef CORE_RESOURCE_LOCATOR_H_
#define CORE_RESOURCE_LOCATOR_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common header ***/
#include "Defines.h"
	/*** extra headers if needed ***/
#include "Subsystems/Graphics/TextureCache.h"
	/*** end header inclusion ***/

namespace Core
{
	class ResourceLocator
	{
	private:
		Graphics::TextureCache* _textureCache;

	public:
		ResourceLocator();

		void Register(Graphics::TextureCache& cache);
		Graphics::TextureCache& getTextureCache() const;
	};
}

#endif //CORE_RESOURCE_LOCATOR_H_