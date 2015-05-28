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

namespace core
{
   enum VertexTopology;

   struct Mesh
   {
      HVertexShader vshader;
      HPixelShader pshader;
      HTexture texture;
      VertexTopology topology;
      HealthVertexBuffer vertices;
      IndexBuffer indices;
   };
   
   Mesh makeSolidQuad(vec2f pos, vec2f hs, HVertexShader vshader, HPixelShader pshader);
   Mesh makeOutlineQuad(vec2f pos, vec2f hs, HVertexShader vshader, HPixelShader pshader);
   Mesh makeTexturedQuad(vec2f pos, vec2f hs, HTexture texture, vec2f topLeftUV, vec2f botRightUV, HVertexShader vshader, HPixelShader pshader);
   // rounds 'points' to the next power of 2
   Mesh makeSolidCircle(vec2f pos, float radius, uint32_t points, HVertexShader vshader, HPixelShader pshader);
   // rounds 'points' to the next power of 2
   Mesh makeOutlineCircle(vec2f pos, float radius, uint32_t points, HVertexShader vshader, HPixelShader pshader);
}
