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
#include <Common/Cache.h>
#include <Common/Handle.h>
/******* end header inclusion *******/

namespace Core
{
   struct tag_DXTexture;
   typedef Handle<tag_DXTexture> HTexture;
   
   struct DXTexture
   {
      ID3D11Texture2D* rawTexture;
      uint32_t width;
      uint32_t height;
   };

   class DXTextureManager
   {
   public:
      

   private:
      typedef Cache<DXTexture, HTexture> DXTextureCache;
      
      DXTextureCache m_textures;
      DXTexture m_defaultTexture;
   };
}
