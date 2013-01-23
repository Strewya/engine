//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Subsystems/Graphics/TextureInfo.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Graphics
{
	TextureInfo::TextureInfo()
		: _filename(), _width(0), _height(0)
	{}

	TextureInfo::TextureInfo(const char* filename, uint width, uint height)
		: _filename(filename), _width(width), _height(height)
    {}

	TextureInfo::TextureInfo(const String& filename, uint width, uint height)
		: _filename(filename), _width(width), _height(height)
    {}
    
	const String& TextureInfo::getFilename() const
	{
		return _filename;
	}

	uint TextureInfo::getWidth() const
	{
		return _width;
	}

	uint TextureInfo::getHeight() const
	{
		return _height;
	}

	void TextureInfo::setFilename(const char* filename)
	{
		_filename = filename;
	}

	void TextureInfo::setFilename(const String& filename)
	{
		_filename = filename;
	}

	void TextureInfo::setWidth(uint w)
	{
		_width = w;
	}

	void TextureInfo::setHeight(uint h)
	{
		_height = h;
	}

}