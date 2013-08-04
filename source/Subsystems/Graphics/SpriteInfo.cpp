//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Subsystems/Graphics/SpriteInfo.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Graphics
{
	SpriteInfo::SpriteInfo()
		: _name(), _srcRect()
    {}

	SpriteInfo::SpriteInfo(const char* name, const Util::Rect& srcRect)
		: _name(name), _srcRect(srcRect)
    {}

	SpriteInfo::SpriteInfo(const String& name, const Util::Rect& srcRect)
		: _name(name), _srcRect(srcRect)
    {}

	const String& SpriteInfo::getName() const
	{
		return _name;
	}
    
    const Util::Rect& SpriteInfo::getSrcRect() const
	{
		return _srcRect;
	}

	void SpriteInfo::setName(const char* name)
	{
		_name = name;
	}

	void SpriteInfo::setName(const String& name)
	{
		_name = name;
	}

	void SpriteInfo::setSrcRect(const Util::Rect& srcRect)
	{
		_srcRect = srcRect;
	}
}
