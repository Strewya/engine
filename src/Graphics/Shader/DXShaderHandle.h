#pragma once
/********************************************
*  contents:   DXVertexShader, DXPixelShader
*  usage:
********************************************/
/******* C++ headers *******/
#include <string>
/******* common headers *******/
/******* extra headers *******/
#include <Util/Handle.h>
/******* end header inclusion *******/

namespace Core
{
   struct tag_DXVertexShader;
   typedef Handle<tag_DXVertexShader> HVertexShader;

   struct tag_DXPixelShader;
   typedef Handle<tag_DXPixelShader> HPixelShader;
}
