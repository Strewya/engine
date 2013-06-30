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

	auto DXTextureCache::Load(const char* filename, const LoadArgs* loadArgs) -> AssetType*
	{
		//the struct for reading bitmap file info
		D3DXIMAGE_INFO info;
		HRESULT result;
		TextureLoadArgs args;
		if(loadArgs == nullptr)
		{
			args = *loadArgs;
		}

		//create the new texture by loading a bitmap image file
		result = D3DXGetImageInfoFromFile(filename, &info);
		if(SUCCEEDED(result))
		{
			AssetType* assetPtr = nullptr;
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
				&assetPtr);					//destination texture
			if(SUCCEEDED(result))
			{
				return assetPtr;
			}
		}
		return nullptr;
	}

	bool DXTextureCache::Unload(AssetType*& assetPtr)
	{
		if(assetPtr == nullptr)
		{
			return false;
		}
		assetPtr->Release();
		assetPtr = nullptr;
		return true;
	}

	TextureData* DXTextureCache::LoadTexture(const char* name, const Util::Color& transparentKey)
	{
		TextureLoadArgs args = {MakeCOLOR(transparentKey)};
		InstanceID id;
		auto* texture = Acquire(name, id, &args);
		
	}

	TextureData* DXTextureCache::LoadTexture(const String& name, const Util::Color& transparentKey)
	{
		return LoadTexture(name.c_str(), transparentKey);
	}

	TextureData* DXTextureCache::getTexture(uint32_t handle)
	{

	}
}