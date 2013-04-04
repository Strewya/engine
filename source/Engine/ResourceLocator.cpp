//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Engine/ResourceLocator.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Core
{
	ResourceLocator::ResourceLocator()
		: _textureCache(nullptr), _spritesheetCache(nullptr)
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

	void ResourceLocator::Register(Graphics::SpritesheetCache& cache)
	{
		_spritesheetCache = &cache;
	}

	Graphics::SpritesheetCache& ResourceLocator::getSpritesheetCache() const
	{
		return *_spritesheetCache;
	}
}