#ifndef GRAPHICS_TEXTUREINFO_H_
#define GRAPHICS_TEXTUREINFO_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Graphics
{
	class TextureInfo
	{
	public:
		TextureInfo();
		TextureInfo(const char* filename, uint width, uint height);
		TextureInfo(const String& filename, uint width, uint height);

		const String& getFilename() const;
		uint getWidth() const;
		uint getHeight() const;

		void setFilename(const char* filename);
		void setFilename(const String& filename);
		void setWidth(uint w);
		void setHeight(uint h);

	private:
		String _filename;
		uint _width, _height;
	};
}

#endif //GRAPHICS_TEXTUREINFO_H_