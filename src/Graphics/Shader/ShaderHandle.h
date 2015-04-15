#pragma once
/********************************************
*  contents:   HVertexShader, HPixelShader
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
   struct tag_VertexShader;
   typedef Handle<tag_VertexShader> HVertexShader;

   struct tag_PixelShader;
   typedef Handle<tag_PixelShader> HPixelShader;
}
