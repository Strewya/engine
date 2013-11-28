//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Subsystems/Graphics/DirectX/DXTexture.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Graphics
{
	DXTexture::DXTexture()
		: lpd3dTexture(nullptr)
	{}

	DXTexture::DXTexture(uint32_t w, uint32_t h, LPDIRECT3DTEXTURE9 lpTexture)
		: TextureData(w,h), lpd3dTexture(lpTexture)
	{}

	DXTexture::~DXTexture()
	{
		Release();
	}

	void DXTexture::Release()
	{
		if(lpd3dTexture)
		{
			lpd3dTexture->Release();
			lpd3dTexture = nullptr;
		}
	}
}
