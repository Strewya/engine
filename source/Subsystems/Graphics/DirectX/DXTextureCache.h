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
		void setD3DDevice(LPDIRECT3DDEVICE9 d3ddev);
		
		InstanceID loadTexture(const char* name, const Util::Color& transparentKey, TextureData** outPtr);
		TextureData* getTexture(InstanceID handle);
		InstanceID getTexture(const char* name, TextureData** outPtr);
		void clearTextures();
		bool destroyTexture(InstanceID handle);

	protected:
		bool loadAsset(const char* filename, const LoadArgs* loadArgs, AssetPtr& asset);

	private:
		LPDIRECT3DDEVICE9 _d3ddev;
	};
}