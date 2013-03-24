#ifndef SUBSYSTEMS_GRAPHICS_TEXTURE_H_
#define SUBSYSTEMS_GRAPHICS_TEXTURE_H_
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
	class IRenderer;
}

namespace Graphics
{
	class Texture
	{
	public:
		Texture();
		Texture(const char* filename, uint width, uint height, IRenderer* gfx = nullptr, uint dataHandle = 0);
		Texture(const String& filename, uint width, uint height, IRenderer* gfx = nullptr, uint dataHandle = 0);
		Texture(const Texture& rhs);
		Texture& operator=(const Texture& rhs);
		~Texture();

		const String& getFilename() const;
		uint getWidth() const;
		uint getHeight() const;
		uint getDataHandle() const;

		void setFilename(const char* filename);
		void setFilename(const String& filename);
		void setWidth(uint w);
		void setHeight(uint h);
		void setSize(uint width, uint height);

	private:
		String _filename;
		uint _width, _height;
		uint _dataHandle;
		IRenderer* _gfx;
	};
}

#endif //SUBSYSTEMS_GRAPHICS_TEXTURE_H_