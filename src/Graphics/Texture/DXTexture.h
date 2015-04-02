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
      std::string name;
      ID3D11ShaderResourceView* shaderResourceView;
      uint32_t width;
      uint32_t height;
   };
}
