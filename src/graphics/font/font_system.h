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
   struct IndexBuffer;
   struct VertexBuffer;
   
   enum TextJustification;

   u32 countVisibleCharacters(const char* text);
   u32 calculateVertexCount(u32 visibleCharacters);
   u32 calculateIndexCount(u32 visibleCharacters);

   // #todo potentially more than bool return codes, like bufferTooSmall or something...
   bool makeTextMesh(VertexBuffer& vb, IndexBuffer& ib, const char* text, const FontDescriptor& fd, v2 scale, TextJustification justify_x, TextJustification justify_y);
   bool makeTextMesh(VertexBuffer& vb, IndexBuffer& ib, const char* text, const FontDescriptor& fd, v2 scale, TextJustification justify_x, TextJustification justify_y, Rect clipBox);

}