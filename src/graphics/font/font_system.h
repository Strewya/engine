#pragma once
/********************************************
*  contents:   #description
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
#include "graphics/shader/shader_handle.h"
#include "graphics/texture/texture_cache.h"
/******* end header inclusion *******/

namespace core
{
   struct FontDescriptor;
   struct GraphicsSystem;
   struct Mesh;
   struct Rect;

   enum TextJustification;

   Mesh makeMesh_text(GraphicsSystem* gfx, const char* text, const FontDescriptor& fd, v2 scale, TextJustification justify_x, TextJustification justify_y);
   Mesh makeMesh_text(GraphicsSystem* gfx, const char* text, const FontDescriptor& fd, v2 scale, TextJustification justify_x, TextJustification justify_y, Rect clipBox);

}