//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "graphics/texture/texture_file_loader.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "graphics/texture/texture.h"
#include "util/utility.h"
/******* end headers *******/

namespace core
{
   bool TextureFileLoader::init(ID3D11Device* device)
   {
      CORE_INIT_START(TextureFileLoader);

      m_dev = device;

      CORE_STATUS_AND(m_dev != nullptr);

      CORE_INIT_END(TextureFileLoader);
   }

   bool TextureFileLoader::shutdown()
   {
      CORE_SHUTDOWN_START(TextureFileLoader);

      m_dev = nullptr;

      CORE_SHUTDOWN_END(TextureFileLoader);
   }

   Texture TextureFileLoader::load(const char* filename) const
   {
      Texture result{0, 0, nullptr};
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
