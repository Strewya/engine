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
      VertexTopology topology;
      HealthVertexBuffer vertices;
      IndexBuffer indices;
   };


   Mesh makeSolidQuad(v2 pos, v2 hs);
   Mesh makeSolidQuad(Rect rect);
   Mesh makeOutlineQuad(v2 pos, v2 hs);
   Mesh makeOutlineQuad(Rect rect);
   Mesh makeTexturedQuad(v2 pos, v2 hs, v2 topLeftUV, v2 botRightUV);
   Mesh makeTexturedQuad(Rect rect, v2 topLeftUV, v2 botRightUV);
   // rounds 'points' to the next power of 2
   Mesh makeSolidCircle(v2 pos, f32 radius, u32 points);
   Mesh makeSolidCircle(Circle circle, u32 points);
   // rounds 'points' to the next power of 2
   Mesh makeOutlineCircle(v2 pos, f32 radius, u32 points);
   Mesh makeOutlineCircle(Circle circle, u32 points);

   Mesh makeLine(v2 a, v2 b);
}
