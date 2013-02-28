//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "ResourceLocator.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Core
{
	ResourceLocator::ResourceLocator()
	{
	}

	void ResourceLocator::Register(Graphics::TextureCache& cache)
	{
		_textureCache = &cache;
	}

	Graphics::TextureCache& ResourceLocator::getTextureCache() const
	{
		return *_textureCache;
	}
}