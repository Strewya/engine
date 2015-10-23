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
      u32 referenceId;
      u32 width;
      u32 height;
      ID3D11ShaderResourceView* _shaderResourceView;

      bool loaded()
      {
         auto result = _shaderResourceView != nullptr && width > 0 && height > 0;
         return result;
      }

      bool unloaded()
      {
         auto result = _shaderResourceView == nullptr && width == 0 && height == 0;
         return result;
      }
   };
}
