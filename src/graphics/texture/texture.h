#pragma once
/********************************************
*  contents:   texture resource type
*  usage:
********************************************/
/******* c++ headers *******/
#include <cstdint>
/******* common headers *******/
#include "graphics/dx_include.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   struct Texture
   {
      uint32_t width;
      uint32_t height;
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
