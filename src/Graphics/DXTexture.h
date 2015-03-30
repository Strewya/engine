#pragma once
/********************************************
*  contents:   DXTexture
*  usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
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
   };
}
