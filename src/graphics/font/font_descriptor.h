#pragma once
/********************************************
*  contents:   #description
*  usage:      
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
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
      // #todo replace with material
      HTexture fontTexture;
      HVertexShader vshader;
      HPixelShader pshader;
      Rect glyphs[128 - 32];
      u32 height;
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