#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Subsystems/Graphics/DirectX/DXInclude.h"
#include "Util/AssetStorage.h"
	/*** end header inclusion ***/

namespace Graphics
{
	struct TextureLoadArgs
	{
		D3DCOLOR _transparency;
	};

	class DXTextureAsset : public Util::AssetStore<IDirect3DTexture9, TextureLoadArgs>
	{
	public:
		void SetD3DDevice(LPDIRECT3DDEVICE9 d3ddev);

	protected:
		AssetType* Load(const char* filename, const LoadArgs* loadArgs);
		bool Unload(AssetType*& assetPtr);

	private:
		LPDIRECT3DDEVICE9 _d3ddev;
	};
}