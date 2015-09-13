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
   struct Mesh;
   struct Rect;

   enum TextJustification;

   struct FontSystem
   {
   public:
      bool init(LinearAllocator& a, TextureCache& textures);
      bool shutdown();

      Mesh makeTextMesh(const char* text, const FontDescriptor& fd, v2 scale, TextJustification justify_x, TextJustification justify_y);
      Mesh makeTextMesh(const char* text, const FontDescriptor& fd, v2 scale, TextJustification justify_x, TextJustification justify_y, Rect clipBox);

   private:
      StackAllocator m_allocator;
      TextureCache* m_textures;
   };

}