//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Subsystems/Graphics/DirectX/DXTextureCache.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Graphics
{
	void DXTextureCache::SetD3DDevice(LPDIRECT3DDEVICE9 d3ddev)
	{
		_d3ddev = d3ddev;
	}

	bool DXTextureCache::Load(const char* filename, const LoadArgs* loadArgs, AssetPtr& outAsset)
	{
		//the struct for reading bitmap file info
		D3DXIMAGE_INFO info;
		HRESULT result;
		TextureLoadArgs args;
		if(loadArgs != nullptr)
		{
			args = *loadArgs;
		}

		//create the new texture by loading a bitmap image file
		result = D3DXGetImageInfoFromFile(filename, &info);
		if(SUCCEEDED(result))
		{
			LPDIRECT3DTEXTURE9 lpTexture = nullptr;
			result = D3DXCreateTextureFromFileEx(
				_d3ddev,					//direct3D device object
				filename,					//the image filename
				info.Width,					//the image width
				info.Height,				//the image height
				1,							//mip-map levels (1 for no chain)
				D3DPOOL_DEFAULT,			//the type of surface (default)
				D3DFMT_UNKNOWN,				//texture format
				D3DPOOL_MANAGED,			//memory class for the image
				D3DX_DEFAULT,				//image filter
				D3DX_DEFAULT,				//mip filter
				args._transparency,			//color key for transparency
				&info,						//bitmap file info (from loaded file)
				nullptr,					//color palette
				&lpTexture);				//destination texture
			if(SUCCEEDED(result))
			{
				outAsset.reset(new DXTexture(info.Width, info.Height, lpTexture));
			}
		}
		return SUCCEEDED(result);
	}

	bool DXTextureCache::Unload(AssetPtr& assetPtr)
	{
		if(assetPtr == nullptr)
		{
			return false;
		}
		assetPtr->lpd3dTexture->Release();
		assetPtr.reset(nullptr);
		return true;
	}

	InstanceID DXTextureCache::LoadTexture(const char* name, const Util::Color& transparentKey, TextureData** outPtr)
	{
		TextureLoadArgs args = {MakeCOLOR(transparentKey)};
		InstanceID id = NOT_FOUND;
		auto* asset = Acquire(name, &args);
		if(asset != nullptr)
		{
			id = asset->id;
			if(outPtr != nullptr)
			{
				*outPtr = asset->dataPtr.get();
			}
		}
		return id;
	}
		
	TextureData* DXTextureCache::getTexture(uint32_t handle)
	{
		auto* asset = CheckLoaded(handle);
		TextureData* data = nullptr;
		if(asset != nullptr)
		{
			data = asset->dataPtr.get();
		}
		return data;
	}

	InstanceID DXTextureCache::getTexture(const char* name, TextureData** outPtr)
	{
		auto* asset = CheckLoaded(name);
		InstanceID id = NOT_FOUND;
		if(asset != nullptr)
		{
			id = asset->id;
			if(outPtr != nullptr)
			{
				*outPtr = asset->dataPtr.get();
			}
		}
		return id;
	}
}