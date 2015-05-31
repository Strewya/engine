#pragma once
/********************************************
*  contents:   #description
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include "graphics/shader/shader_handle.h"
#include "graphics/texture/texture_manager.h"
/******* end header inclusion *******/

namespace core
{
   struct FontDescriptor;
   struct Mesh;
   struct Rect;
   struct Vec2;

   enum TextJustification;

   struct FontSystem
   {
   public:
      bool init(const TextureManager& textures);
      bool shutdown();

      Mesh makeTextMesh(const char* text, const FontDescriptor& fd, Vec2 scale, TextJustification justify_x, TextJustification justify_y);
      Mesh makeTextMesh(const char* text, const FontDescriptor& fd, Vec2 scale, TextJustification justify_x, TextJustification justify_y, Rect clipBox);

   private:
      const TextureManager* m_textures;
   };

}