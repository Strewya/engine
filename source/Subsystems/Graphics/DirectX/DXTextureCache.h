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
#include "Subsystems/Graphics/ITextureCache.h"
#include "Subsystems/Graphics/DirectX/DXTexture.h"
	/*** end header inclusion ***/

namespace Util
{
	class Color;
}

namespace Graphics
{
	struct TextureLoadArgs
	{
		D3DCOLOR _transparency;
	};

	class DXTextureCache : public Util::AssetStore<DXTexture, TextureLoadArgs>, public ITextureCache
	{
	public:
		DXTextureCache();
		void SetD3DDevice(LPDIRECT3DDEVICE9 d3ddev);
		
		InstanceID LoadTexture(const char* name, const Util::Color& transparentKey, TextureData** outPtr);
		TextureData* getTexture(uint32_t handle);
		InstanceID getTexture(const char* name, TextureData** outPtr);

	protected:
		bool Load(const char* filename, const LoadArgs* loadArgs, AssetPtr& asset);
		bool Unload(AssetPtr& assetPtr);

	private:
		LPDIRECT3DDEVICE9 _d3ddev;
	};
}