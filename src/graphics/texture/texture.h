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
      u32 width;
      u32 height;
      ID3D11ShaderResourceView* _shaderResourceView;

      bool loaded()
      {
         return _shaderResourceView != nullptr && width > 0 && height > 0;
      }

      bool unloaded()
      {
         return _shaderResourceView == nullptr && width == 0 && height == 0;
      }
   };
}
