#pragma once
/********************************************
*  contents:   mesh type used for rendering
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
#include "graphics/shader/shader_handle.h"
#include "graphics/texture/texture_handle.h"
#include "graphics/graphics_types.h"
/******* end header inclusion *******/

namespace core
{
   struct Circle;
   struct Rect;

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
   
   Mesh makeSolidQuad(v2 pos, v2 hs, HVertexShader vshader, HPixelShader pshader);
   Mesh makeSolidQuad(Rect rect, HVertexShader vshader, HPixelShader pshader);
   Mesh makeOutlineQuad(v2 pos, v2 hs, HVertexShader vshader, HPixelShader pshader);
   Mesh makeOutlineQuad(Rect rect, HVertexShader vshader, HPixelShader pshader);
   Mesh makeTexturedQuad(v2 pos, v2 hs, HTexture texture, v2 topLeftUV, v2 botRightUV, HVertexShader vshader, HPixelShader pshader);
   Mesh makeTexturedQuad(Rect rect, HTexture texture, v2 topLeftUV, v2 botRightUV, HVertexShader vshader, HPixelShader pshader);
   // rounds 'points' to the next power of 2
   Mesh makeSolidCircle(v2 pos, f32 radius, u32 points, HVertexShader vshader, HPixelShader pshader);
   Mesh makeSolidCircle(Circle circle, u32 points, HVertexShader vshader, HPixelShader pshader);
   // rounds 'points' to the next power of 2
   Mesh makeOutlineCircle(v2 pos, f32 radius, u32 points, HVertexShader vshader, HPixelShader pshader);
   Mesh makeOutlineCircle(Circle circle, u32 points, HVertexShader vshader, HPixelShader pshader);

   Mesh makeLine(v2 a, v2 b, HVertexShader vshader, HPixelShader pshader);
}
