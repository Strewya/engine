#pragma once
/********************************************
*  contents:   pixel shader resource type
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "graphics/dx_include.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   struct PixelShader
   {
      ID3D11PixelShader* _pixel;

      bool loaded()
      {
         return _pixel != nullptr;
      }

      bool unloaded()
      {
         return !loaded();
      }
   };
}
