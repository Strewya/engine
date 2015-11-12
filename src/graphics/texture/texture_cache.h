#pragma once
/********************************************
*  contents:   texture resource manager
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
#include "graphics/texture/texture_handle.h"
/******* end header inclusion *******/

namespace core
{
   struct Memory;
   struct Texture;
   struct TextureCache;

   namespace cache
   {
      void init(TextureCache* cache, Memory& m, u32 maxSlots);
      void shutdown(TextureCache* cache);

      HTexture insert(TextureCache* cache, Texture asset);
      Texture remove(TextureCache* cache, HTexture handle);
      Texture get(TextureCache* cache, HTexture handle);
      u32 getUsedCount(TextureCache* cache);
      HTexture find(TextureCache* cache, u32 assetId);
   }
}
