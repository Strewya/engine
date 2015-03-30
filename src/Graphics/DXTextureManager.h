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
      bool init(DXTexture defaultData);
      bool shutdown();

      HTexture loadFromFile(const std::string& filename);
      DXTexture& getData(HTexture handle);
      void release(HTexture handle);

   private:
      typedef Cache<DXTexture, HTexture> Data;
      typedef NameCache<HTexture> Names;

      Data m_data;
      Names m_names;
      DXTexture m_default;
      DXTextureFileLoader m_fileloader;
   };
}
