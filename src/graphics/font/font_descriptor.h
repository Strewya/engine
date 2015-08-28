#pragma once
/********************************************
*  contents:   #description
*  usage:      
********************************************/
/******* c++ headers *******/
#include <cstdint>
/******* common headers *******/
/******* extra headers *******/
#include "graphics/texture/texture_handle.h"
#include "graphics/shader/shader_handle.h"
#include "utility/geometry/rect.h"
/******* end header inclusion *******/

namespace core
{
   struct LuaStack;

   struct FontDescriptor
   {
      HTexture fontTexture;
      HVertexShader vshader;
      HPixelShader pshader;
      Rect glyphs[128 - 32];
      uint32_t height;
   };

   enum TextJustification
   {
      Left = 0,
      Center = 1,
      Right = 2,
      Top = Left,
      Bottom = Right,
      Middle = Center
   };

   FontDescriptor loadFont(LuaStack lua, const char* filename, HTexture texture, HVertexShader vshader, HPixelShader pshader);
}