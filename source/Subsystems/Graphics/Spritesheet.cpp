//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Subsystems/Graphics/Spritesheet.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Graphics
{
	Spritesheet::Spritesheet()
	{
		_sprites.reserve(16);
	}

	Spritesheet::Spritesheet(const char* name)
		: _sheetName(name)
	{
		_sprites.reserve(16);
	}

	Spritesheet::Spritesheet(const String& name)
		: _sheetName(name)
	{
		_sprites.reserve(16);
	}

	Spritesheet::~Spritesheet()
	{}

	bool Spritesheet::Valid(uint index) const
	{
		return (0 <= index && index < _sprites.size());
	}
	
	int Spritesheet::getHandle(const char* name) const
	{
		for(auto it = _sprites.begin(); it != _sprites.end(); ++it)
		{
			if(it->getName().compare(name) == 0)
			{
				return it - _sprites.begin();
			}
		}
		return NOT_FOUND;
	}

	int Spritesheet::getHandle(const String& name) const
	{
		return getHandle(name.c_str());
	}
		
	const SpriteInfo& Spritesheet::getSprite(uint index) const
	{
		if(Valid(index))
		{
			return _sprites[index];
		}
		throw std::exception("Attempted to dereference inexisting texture");
	}

	uint Spritesheet::Insert(const SpriteInfo& sprite)
	{
		if(sprite.getName().empty())
		{
			return NOT_FOUND;
		}
		int index = getHandle(sprite.getName().c_str());
		if(index != NOT_FOUND)
		{
			return index;
		}
		index = _sprites.size();
		_sprites.push_back(sprite);
		return index;
	}

	const Texture& Spritesheet::getTexture() const
	{
		return _texture;
	}

	const String& Spritesheet::getTextureName() const
	{
		return _textureName;
	}

	const String& Spritesheet::getSpritesheetName() const
	{
		return _sheetName;
	}

	void Spritesheet::setTextureName(const char* name)
	{
		_textureName = name;
	}

	void Spritesheet::setTextureName(const String& name)
	{
		_textureName = name;
	}

	void Spritesheet::setTexture(const Texture& texture)
	{
		_texture = texture;
	}

	void Spritesheet::setSpritesheetName(const char* name)
	{
		_sheetName = name;
	}

	void Spritesheet::setSpritesheetName(const String& name)
	{
		_sheetName = name;
	}
}