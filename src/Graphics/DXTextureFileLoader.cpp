//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Graphics/DXTextureFileLoader.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Graphics/DXTexture.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
   bool DXTextureFileLoader::init(ID3D11Device* device)
   {
      m_dev = device;

      CORE_STATUS(m_dev != nullptr);
      CORE_INIT(DXTextureFileLoader);
   }

   bool DXTextureFileLoader::shutdown()
   {
      m_dev = nullptr;

      CORE_STATUS(true);
      CORE_SHUTDOWN(DXTextureFileLoader);
   }

   DXTexture DXTextureFileLoader::load(const std::string& filename) const
   {
      DXTexture result{filename, nullptr, 0, 0};
      HRESULT hr = D3DX11CreateShaderResourceViewFromFile(m_dev, filename.c_str(), nullptr, nullptr, &result.shaderResourceView, nullptr);
      if( SUCCEEDED(hr) )
      {
         ID3D11Resource* res = nullptr;
         result.shaderResourceView->GetResource(&res);

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
            CORE_INFO("Failed to read dimensions for texture '", filename, "'");
         }
         safeRelease(texture2d);
         safeRelease(res);
      }
      else
      {
         CORE_INFO("Failed to load texture '", filename, "'");
      }
      return result;
   }

   void DXTextureFileLoader::unload(DXTexture& texture) const
   {
      safeRelease(texture.shaderResourceView);
      texture.width = 0;
      texture.height = 0;
   }
}
