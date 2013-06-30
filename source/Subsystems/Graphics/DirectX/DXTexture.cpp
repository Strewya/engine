//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Subsystems/Graphics/DirectX/DXTexture.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Graphics
{
	DXTexture::DXTexture()
		: d3dData(nullptr)
	{}

	DXTexture::~DXTexture()
	{
		Release();
	}

	void DXTexture::Release()
	{
		if(d3dData)
		{
			d3dData->Release();
			d3dData = nullptr;
		}
	}
}