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
