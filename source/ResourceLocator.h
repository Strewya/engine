#ifndef CORE_RESOURCELOCATOR_H_
#define CORE_RESOURCELOCATOR_H_
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

#endif //CORE_RESOURCELOCATOR_H_