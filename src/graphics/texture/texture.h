#pragma once
/********************************************
*  contents:   texture resource type
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "graphics/dx_include.h"
#include "utility/types.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   struct Texture
   {
      ID3D11ShaderResourceView* _shaderResourceView;
      u32 assetId;
      u32 width;
      u32 height;
   };

   inline bool isLoaded(Texture t)
   {
      auto result = (t._shaderResourceView != nullptr);
      return result;
   }

   inline bool isUnloaded(Texture t)
   {
      auto result = (t._shaderResourceView == nullptr);
      return result;
   }

   inline u32 getAssetId(Texture t)
   {
      auto result = t.assetId;
      return result;
   }
}
