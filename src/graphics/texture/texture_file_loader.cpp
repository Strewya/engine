//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "graphics/texture/texture_file_loader.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "graphics/texture/texture.h"
#include "utility/utility.h"
/******* end headers *******/

namespace core
{
   void TextureFileLoader::init(ID3D11Device* device)
   {
      m_dev = device;
      CORE_ASSERT_DBGERR(m_dev != nullptr, "D3D11 device is NULL in TextureFileLoader!");
   }

   Texture TextureFileLoader::load(const char* filename) const
   {
      Texture result{nullptr, 0, 0, 0};
      HRESULT hr = D3DX11CreateShaderResourceViewFromFile(m_dev, filename, nullptr, nullptr, &result._shaderResourceView, nullptr);
      if( SUCCEEDED(hr) )
      {
         ID3D11Resource* res = nullptr;
         result._shaderResourceView->GetResource(&res);

         ID3D11Texture2D* texture2d = nullptr;
         hr = res->QueryInterface(&texture2d);
         if( SUCCEEDED(hr) )
         {
            D3D11_TEXTURE2D_DESC desc;
            texture2d->GetDesc(&desc);
            result.width = desc.Width;
            result.height = desc.Height;
         }
         else
         {
            CORE_LOG("Failed to read dimensions for texture '", filename, "'");
         }
         safeRelease(texture2d);
         safeRelease(res);
      }
      else
      {
         CORE_LOG("Failed to load texture '", filename, "'");
      }
      return result;
   }

   void TextureFileLoader::unload(Texture& data) const
   {
      safeRelease(data._shaderResourceView);
      data.width = 0;
      data.height = 0;
   }
}
