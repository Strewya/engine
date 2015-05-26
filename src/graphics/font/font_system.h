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
#include "util/geometry/vec2_fwd.h"
/******* end header inclusion *******/

namespace core
{
   struct FontDescriptor;
   struct Mesh;
   struct Rect;

   struct FontSystem
   {
   public:
      bool init(const TextureManager& textures);
      bool shutdown();

      Mesh makeTextMesh(const char* text, const FontDescriptor& fd, vec2f scale, vec2i textJustification);
      Mesh makeTextMesh(const char* text, const FontDescriptor& fd, vec2f scale, vec2i textJustification, Rect clipBox);

   private:
      const TextureManager* m_textures;
   };

}