//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Subsystems/Graphics/TextureCache.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Subsystems/Graphics/Interface.h"
	/*** end headers ***/

namespace Graphics
{
	TextureCache::TextureCache()
		: _parent(nullptr), _gfx(nullptr)
	{
	}

	TextureCache::~TextureCache()
	{
	}

	void TextureCache::setReferences(TextureCache& parent, Interface& gfx)
	{
		if(this != &parent)
		{
			_parent = &parent;
		}
		_gfx = &gfx;
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
			Texture& tex = _parent->getTexture(name);
			_textures.push_back(tex);
			return tex;
		}
		//we have no parent, meaning we are top layer, so we ask to load the texture
		
	}
}