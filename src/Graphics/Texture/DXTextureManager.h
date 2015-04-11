#pragma once
/********************************************
*  contents:   DXTextureManager
*  usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
/******* common headers *******/
#include <Graphics/DXInclude.h>
/******* extra headers *******/
#include <Graphics/Texture/DXTexture.h>
#include <Graphics/Texture/DXTextureFileLoader.h>
#include <Graphics/Texture/DXTextureHandle.h>
#include <Util/Cache.h>
#include <Util/NameCache.h>
/******* end header inclusion *******/

namespace Core
{
   class DXTextureManager
   {
   public:
      bool init(ID3D11Device* device, DXTexture defaultData);
      bool shutdown();

      HTexture loadFromFile(const std::string& filename);
      DXTexture& getData(HTexture handle);
      void release(HTexture handle);

   private:
      typedef Cache<DXTexture, HTexture> TextureCache;
      typedef NameCache<HTexture> TextureNames;

      TextureCache m_data;
      TextureNames m_names;
      DXTexture m_defaultData;
      DXTextureFileLoader m_fileloader;
   };
}
