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
		: _name(), _italic(false), _size(10), _weight(400)
    {}

	FontInfo::FontInfo(const char* name, uint32_t size, bool italic, uint32_t weight)
		: _name(name), _italic(italic), _size(size), _weight(weight)
    {}
	
	FontInfo::FontInfo(const String& name, uint32_t size, bool italic, uint32_t weight)
		: _name(name), _italic(italic), _size(size), _weight(weight)
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