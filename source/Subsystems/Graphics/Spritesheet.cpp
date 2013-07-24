//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Subsystems/Graphics/Spritesheet.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Graphics
{
	Spritesheet::Spritesheet()
	{
	}

	Spritesheet::Spritesheet(const char* name)
		: _sheetName(name), _textureHandle(NOT_FOUND)
	{
	}

	Spritesheet::Spritesheet(const String& name)
		: _sheetName(name), _textureHandle(NOT_FOUND)
	{
	}

	Spritesheet::~Spritesheet()
	{}

	bool Spritesheet::SpriteValid(uint32_t index) const
	{
		return (0 <= index && index < _sprites.size());
	}

	bool Spritesheet::AnimationValid(uint32_t index) const
	{
		return (0 <= index && index < _animations.size());
	}
	
	int Spritesheet::getSpriteHandle(const char* name) const
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

	int Spritesheet::getSpriteHandle(const String& name) const
	{
		return getSpriteHandle(name.c_str());
	}
	
	int Spritesheet::getAnimationHandle(const char* name) const
	{
		for(auto it = _animations.begin(); it != _animations.end(); ++it)
		{
			if(it->getName().compare(name) == 0)
			{
				return it - _animations.begin();
			}
		}
		return NOT_FOUND;
	}

	int Spritesheet::getAnimationHandle(const String& name) const
	{
		return getAnimationHandle(name.c_str());
	}

	const SpriteInfo& Spritesheet::getSprite(uint32_t index) const
	{
		if(SpriteValid(index))
		{
			return _sprites[index];
		}
		throw std::exception("Spritesheet::getSprite(): Attempted to get inexisting sprite.");
	}

	const AnimationInfo& Spritesheet::getAnimation(uint32_t index) const
	{
		if(AnimationValid(index))
		{
			return _animations[index];
		}
		throw std::exception("Spritesheet::getAnimation(): Attempted to get inexisting animation");
	}

	uint32_t Spritesheet::Insert(const SpriteInfo& sprite)
	{
		if(sprite.getName().empty())
		{
			return NOT_FOUND;
		}
		int index = getSpriteHandle(sprite.getName().c_str());
		if(index != NOT_FOUND)
		{
			return index;
		}
		index = _sprites.size();
		_sprites.push_back(sprite);
		return index;
	}

	uint32_t Spritesheet::Insert(const AnimationInfo& animation)
	{
		if(animation.getName().empty())
		{
			return NOT_FOUND;
		}
		int index = getAnimationHandle(animation.getName().c_str());
		if(index != NOT_FOUND)
		{
			return index;
		}
		index = _animations.size();
		_animations.push_back(animation);
		return index;
	}

	uint32_t Spritesheet::getTextureHandle() const
	{
		return _textureHandle;
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

	void Spritesheet::setTexture(uint32_t texture)
	{
		_textureHandle = texture;
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