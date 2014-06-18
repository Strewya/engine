//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include <stdafx.h>
	/*** personal header ***/
#include <Engine/ResourceLocator.h>
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Core
{
	ResourceLocator::ResourceLocator()
		: _textureCache(nullptr), _spritesheetCache(nullptr)
	{
	}

	void ResourceLocator::Register(Graphics::ITextureCache& cache)
	{
		_textureCache = &cache;
	}

	Graphics::ITextureCache& ResourceLocator::getTextureCache() const
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