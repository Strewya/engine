#pragma once
/********************************************
*  contents:   #todo
*  usage:
********************************************/
/******* c++ headers *******/
#include <string>
/******* common headers *******/
#include "graphics/dx_include.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   class PixelShader
   {
   public:
      ID3D11PixelShader* pixel;

      bool loaded()
      {
         return pixel != nullptr;
      }

      bool unloaded()
      {
         return !loaded();
      }
   };
}
