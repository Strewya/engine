#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Graphics
{
	class IRenderer;
}

namespace Graphics
{
	class Texture
	{
	public:
		Texture();
		Texture(const char* filename, uint width, uint height, InstanceID dataHandle = 0);
		Texture(const String& filename, uint width, uint height, InstanceID dataHandle = 0);
		
		const String& getFilename() const;
		uint getWidth() const;
		uint getHeight() const;
		InstanceID getDataHandle() const;

	private:
		String _filename;
		uint _width, _height;
		InstanceID _dataHandle;
	};
}