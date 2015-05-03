#pragma once
/********************************************
*  contents:   mesh type used for rendering
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include "graphics/shader/shader_handle.h"
#include "graphics/texture/texture_handle.h"
#include "graphics/graphics_types.h"
#include "util/geometry/vec2_fwd.h"
/******* end header inclusion *******/

namespace Core
{
   enum VertexTopology;

   class Mesh
   {
   public:
      HVertexShader vshader;
      HPixelShader pshader;
      HTexture texture;
      VertexTopology topology;
      HealthVertexBuffer vertices;
      IndexBuffer indices;
   };
   
   Mesh makeSolidQuad(Vec2f pos, Vec2f hs, HVertexShader vshader, HPixelShader pshader);
   Mesh makeTexturedQuad(Vec2f pos, Vec2f hs, HTexture texture, Vec2f topLeftUV, Vec2f botRightUV, HVertexShader vshader, HPixelShader pshader);
   
}
