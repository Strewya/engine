//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Subsystems/Graphics/TextureCache.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "ResourceLocator.h"
#include "ServiceLocator.h"
#include "Subsystems/Graphics/IRenderer.h"
	/*** end headers ***/

namespace Graphics
{
	TextureCache::TextureCache()
		: _parent(nullptr), _gfx(nullptr)
	{
	}

	void TextureCache::setReferences(const Core::ResourceLocator& resources, const Core::ServiceLocator& services)
	{
		if(this != &resources.getTextureCache() && nullptr != &resources.getTextureCache())
		{
			_parent = &resources.getTextureCache();
		}
		_gfx = &services.getGraphics();
	}

	uint TextureCache::getTextureHandle(const String& name)
	{
		return getTextureHandle(name.c_str());
	}

	uint TextureCache::getTextureHandle(const char* name)
	{
		for(auto it = _textures.begin(); it != _textures.end(); ++it)
		{
			if(it->getFilename().compare(name) == 0)
			{
				return it - _textures.begin();
			}
		}
		//texture doesn't exist in this cache, ask the parent to get it or load it
		if(_parent)
		{
			_textures.push_back(_parent->getTexture(name));
		}
		//we have no parent, meaning we are top layer, so we ask to load the texture
		else
		{
			_textures.emplace_back(_gfx->LoadTexture(name));
		}
		return _textures.size()-1;
	}

	Texture& TextureCache::getTexture(uint handle)
	{
		return _textures[handle];
	}

	Texture& TextureCache::getTexture(const String& name)
	{
		return getTexture(name.c_str());
	}

	Texture& TextureCache::getTexture(const char* name)
	{
		for(auto& tex : _textures)
		{
			if(tex.getFilename().compare(name) == 0)
			{
				return tex;
			}
		}
		//texture doesn't exist in this cache, ask the parent to get it or load it
		if(_parent)
		{
			_textures.push_back(_parent->getTexture(name));
		}
		//we have no parent, meaning we are top layer, so we ask to load the texture
		else
		{
			_textures.emplace_back(_gfx->LoadTexture(name));
		}
		return _textures.back();
	}
}