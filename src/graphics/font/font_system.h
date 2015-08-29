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
#include "graphics/texture/texture_manager.h"
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
      bool init(const TextureManager& textures);
      bool shutdown();

      Mesh makeTextMesh(const char* text, const FontDescriptor& fd, v2 scale, TextJustification justify_x, TextJustification justify_y);
      Mesh makeTextMesh(const char* text, const FontDescriptor& fd, v2 scale, TextJustification justify_x, TextJustification justify_y, Rect clipBox);

   private:
      const TextureManager* m_textures;
   };

}