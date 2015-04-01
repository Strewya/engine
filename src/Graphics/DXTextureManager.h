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
#include <Graphics/DXTexture.h>
#include <Graphics/DXTextureFileLoader.h>
#include <Util/Cache.h>
#include <Util/Handle.h>
#include <Util/NameCache.h>
/******* end header inclusion *******/

namespace Core
{
   struct tag_DXTexture;
   typedef Handle<tag_DXTexture> HTexture;

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
      DXTexture m_defaultTexture;
      DXTextureFileLoader m_fileloader;
   };
}
