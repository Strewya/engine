//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Subsystems/Graphics/Texture.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Subsystems/Graphics/Interface.h"
	/*** end headers ***/

namespace Graphics
{
	Texture::Texture()
		: _filename(), _width(0), _height(0), _dataHandle(0), _gfx(nullptr)
	{}

	Texture::Texture(const char* filename, uint width, uint height, Interface* gfx, uint dataHandle)
		: _filename(filename), _width(width), _height(height), _dataHandle(dataHandle), _gfx(gfx)
    {}

	Texture::Texture(const String& filename, uint width, uint height, Interface* gfx, uint dataHandle)
		: _filename(filename), _width(width), _height(height), _dataHandle(dataHandle), _gfx(gfx)
    {}
    
	Texture::Texture(const Texture& rhs)
		: _filename(rhs._filename), _width(rhs._width), _height(rhs._height), _dataHandle(rhs._dataHandle), _gfx(rhs._gfx)
    {
		if(_gfx != nullptr)
		{
			_gfx->LoadTexture(_dataHandle);
		}
	}
	
	Texture& Texture::operator=(const Texture& rhs)
    {
		if(this != &rhs)
		{
			_dataHandle = rhs._dataHandle;
			_filename = rhs._filename;
			_width = rhs._width;
			_height = rhs._height;
			_gfx = rhs._gfx;
			if(_gfx != nullptr)
			{
				_gfx->LoadTexture(_dataHandle);
			}
		}
		return *this;
	}

	Texture::~Texture()
    {
		if(_gfx)
		{
			_gfx->ReleaseTexture(_dataHandle);
		}
	}

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

	uint Texture::getDataHandle() const
	{
		return _dataHandle;
	}

	void Texture::setFilename(const char* filename)
	{
		_filename = filename;
	}

	void Texture::setFilename(const String& filename)
	{
		_filename = filename;
	}

	void Texture::setWidth(uint w)
	{
		_width = w;
	}

	void Texture::setHeight(uint h)
	{
		_height = h;
	}

	void Texture::setSize(uint width, uint height)
	{
		_width = width;
		_height = height;
	}
}