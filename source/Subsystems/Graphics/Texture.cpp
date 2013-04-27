//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Subsystems/Graphics/Texture.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Subsystems/Graphics/IRenderer.h"
	/*** end headers ***/

namespace Graphics
{
	Texture::Texture()
		: _filename(), _width(0), _height(0), _dataHandle(0)
	{}

	Texture::Texture(const char* filename, uint width, uint height, InstanceID dataHandle)
		: _filename(filename), _width(width), _height(height), _dataHandle(dataHandle)
    {}

	Texture::Texture(const String& filename, uint width, uint height, InstanceID dataHandle)
		: _filename(filename), _width(width), _height(height), _dataHandle(dataHandle)
    {}
    
	const String& Texture::getFilename() const
	{
		return _filename;
	}

	uint Texture::getWidth() const
	{
		return _width;
	}

	uint Texture::getHeight() const
	{
		return _height;
	}

	InstanceID Texture::getDataHandle() const
	{
		return _dataHandle;
	}
}