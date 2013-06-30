#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Subsystems/Graphics/TextureData.h"
#include "Subsystems/Graphics/DirectX/DXInclude.h"
	/*** end header inclusion ***/

namespace Graphics
{
	class DXTexture : public TextureData
	{
	public:
		DXTexture();
		~DXTexture();
		
		LPDIRECT3DTEXTURE9 d3dData;

		void Release();
	};
}