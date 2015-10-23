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
   struct GraphicsSystem;

   struct FontDescriptor
   {
      // #todo replace with material
      HTexture fontTexture;
      HVertexShader vshader;
      HPixelShader pshader;
      Rect glyphs[128 - 32];
      u32 height;

      bool loaded()
      {
         auto result = fontTexture.isNull() == false && vshader.isNull() == false && pshader.isNull() == false && height != 0;
         return !result;
      }
      bool unloaded()
      {
         auto result = fontTexture.isNull() && vshader.isNull() && pshader.isNull() && height == 0;
         return result;
      }
   };

   CORE_RESOURCE_HANDLE(HFont);

   enum TextJustification
   {
      Left = 0,
      Center = 1,
      Right = 2,
      Top = Left,
      Bottom = Right,
      Middle = Center
   };

   HFont loadFont(LuaStack lua, GraphicsSystem* gfx, char* definitionFile);
}