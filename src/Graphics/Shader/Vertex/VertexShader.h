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
   class VertexShader
   {
   public:
      ID3D11InputLayout* inputLayout;
      ID3D11VertexShader* vertex;

      bool loaded()
      {
         return inputLayout != nullptr && vertex != nullptr;
      };

      bool unloaded()
      {
         return inputLayout == nullptr && vertex == nullptr;
      }
   };
}
