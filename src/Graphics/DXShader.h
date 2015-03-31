#pragma once
/********************************************
*  contents:   DXVertexShader, DXPixelShader
*  usage:
********************************************/
/******* C++ headers *******/
#include <string>
/******* common headers *******/
#include <Graphics/DXInclude.h>
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   class DXVertexShader
   {
   public:
      std::string name;
      ID3D11VertexShader* shader;
      ID3D11InputLayout* inputLayout;
   };

   class DXPixelShader
   {
   public:
      std::string name;
      ID3D11PixelShader* shader;
   };
}
