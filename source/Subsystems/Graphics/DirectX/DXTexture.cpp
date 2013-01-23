//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Subsystems/Graphics/DirectX/DXTexture.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Graphics
{
	DXTexture::DXTexture()
		: info(), data(nullptr)
	{}

	DXTexture::DXTexture(const char* filename, uint w, uint h, LPDIRECT3DTEXTURE9 data)
		: info(filename, w, h), data(data)
	{}

	DXTexture::DXTexture(const String& filename, uint w, uint h, LPDIRECT3DTEXTURE9 data)
		: info(filename, w, h), data(data)
	{}

	void DXTexture::Release()
	{
		if(data)
		{
			data->Release();
			data = nullptr;
		}
	}
}