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
   class DXShader
   {
   public:
      ID3D11InputLayout* inputLayout;
      ID3D11VertexShader* vertex;
      ID3D11PixelShader* pixel;
   };
}