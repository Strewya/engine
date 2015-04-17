#pragma once
/********************************************
*  contents:   DXTexture
*  usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <string>
/******* common headers *******/
#include <Graphics/DXInclude.h>
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   class DXTexture
   {
   public:
      ID3D11ShaderResourceView* shaderResourceView;
      uint32_t width;
      uint32_t height;

      bool loaded()
      {
         return shaderResourceView != nullptr && width > 0 && height > 0;
      }

      bool unloaded()
      {
         return shaderResourceView == nullptr && width == 0 && height == 0;
      }
   };
}
