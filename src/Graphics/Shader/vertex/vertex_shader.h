#pragma once
/********************************************
*  contents:   #todo
*  usage:
********************************************/
/******* c++ headers *******/
#include <string>
/******* common headers *******/
#include <graphics/dx_include.h>
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
