#ifndef GRAPHICS_DXTEXTURE_H_
#define GRAPHICS_DXTEXTURE_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Subsystems/Graphics/TextureInfo.h"
#include "Subsystems/Graphics/DirectX/DXInclude.h"
	/*** end header inclusion ***/

namespace Graphics
{
	class DXTexture
	{
		
	public:
		DXTexture();
		DXTexture(const char* filename, uint w, uint h, LPDIRECT3DTEXTURE9 data);
		DXTexture(const String& filename, uint w, uint h, LPDIRECT3DTEXTURE9 data);
		
		TextureInfo info;
		LPDIRECT3DTEXTURE9 data;

		void Release();
	};
}

#endif //GRAPHICS_DXTEXTURE_H_