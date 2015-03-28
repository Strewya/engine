//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Graphics/DXTextureFileLoader.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Graphics/DXTexture.h>
/******* end headers *******/

namespace Core
{
   DXTextureFileLoader::DXTextureFileLoader(ID3D11Device* dev)
      : m_dev(dev)
   {
   }

   DXTexture DXTextureFileLoader::load(std::string filename)
   {
      ID3D11ShaderResourceView* texturePtr = nullptr;
      HRESULT hr = D3DX11CreateShaderResourceViewFromFile(m_dev, filename.c_str(), nullptr, nullptr, &texturePtr, nullptr);
      bool loaded = false;
      if( SUCCEEDED(hr) )
      {
         DXTexture 
         DxTexturePtr loadedTexturePtr(texturePtr, releasePtr<ID3D11ShaderResourceView>);
         auto it = std::find_if(m_textures.begin(), m_textures.end(), [](const DxTexturePtr& ptr)
         {
            return ptr == nullptr;
         });
         if( it == m_textures.end() )
         {
            outTexture.m_rawTextureID = m_textures.size();
            m_textures.emplace_back(std::move(loadedTexturePtr));
         }
         else
         {
            outTexture.m_rawTextureID = std::distance(m_textures.begin(), it);
            it->swap(loadedTexturePtr);
         }
         outTexture.m_name = file.getName();
         outTexture.m_fileHash = file.getHash();
         outTexture.m_dimensions = getTextureDimensions(outTexture);
         loaded = true;

      }
      return loaded;
   }
}
