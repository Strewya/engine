//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Subsystems/Graphics/FontInfo.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Graphics
{
	FontInfo::FontInfo()
		: _name(), _italic(false), _weight(400), _size(10)
    {}

	FontInfo::FontInfo(const char* name, uint32_t size, bool italic, uint32_t weight)
		: _name(name), _italic(italic), _weight(weight), _size(size)
    {}
	
	FontInfo::FontInfo(const String& name, uint32_t size, bool italic, uint32_t weight)
		: _name(name), _italic(italic), _weight(weight), _size(size)
    {}

	const String& FontInfo::getName() const
	{
		return _name;
	}
	bool FontInfo::getItalic() const
	{
		return _italic;
	}
	uint32_t FontInfo::getWeight() const
	{
		return _weight;
	}
	uint32_t FontInfo::getSize() const
	{
		return _size;
	}

	void FontInfo::setName(const char* name)
	{
		_name = name;
	}

	void FontInfo::setName(const String& name)
	{
		_name = name;
	}

	void FontInfo::setItalic(bool italic)
	{
		_italic = italic;
	}

	void FontInfo::setWeight(uint32_t weight)
	{
		_weight = weight;
	}

	void FontInfo::setSize(uint32_t size)
	{
		_size = size;
	}
}
