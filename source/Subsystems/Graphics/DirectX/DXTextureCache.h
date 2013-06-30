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
		void SetD3DDevice(LPDIRECT3DDEVICE9 d3ddev);
		TextureData* LoadTexture(const char* name, const Util::Color& transparentKey);
		TextureData* LoadTexture(const String& name, const Util::Color& transparentKey);
		TextureData* getTexture(uint32_t handle);

	protected:
		AssetType* Load(const char* filename, const LoadArgs* loadArgs);
		bool Unload(AssetType*& assetPtr);

	private:
		LPDIRECT3DDEVICE9 _d3ddev;
	};
}