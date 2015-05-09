#pragma once
/********************************************
*  contents:   vertex shader resource type
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "graphics/dx_include.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   struct VertexShader
   {
      ID3D11InputLayout* _inputLayout;
      ID3D11VertexShader* _vertex;

      bool loaded()
      {
         return _inputLayout != nullptr && _vertex != nullptr;
      };

      bool unloaded()
      {
         return _inputLayout == nullptr && _vertex == nullptr;
      }
   };
}
