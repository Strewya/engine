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
   struct GraphicsSystem;
   struct Rect;

   enum VertexTopology;

   struct Mesh
   {
      VertexTopology topology;
      HealthVertexBuffer vertices;
      IndexBuffer indices;
   };


   Mesh makeMesh_solidQuad(GraphicsSystem* gfx, v2 pos, v2 hs);
   Mesh makeMesh_solidQuad(GraphicsSystem* gfx, Rect rect);

   Mesh makeMesh_outlinedQuad(GraphicsSystem* gfx, v2 pos, v2 hs);
   Mesh makeMesh_outlinedQuad(GraphicsSystem* gfx, Rect rect);

   Mesh makeMesh_texturedQuad(GraphicsSystem* gfx, v2 pos, v2 hs, v2 topLeftUV, v2 botRightUV);
   Mesh makeMesh_texturedQuad(GraphicsSystem* gfx, Rect rect, v2 topLeftUV, v2 botRightUV);
   
   // rounds 'points' to the next power of 2
   Mesh makeMesh_solidCircle(GraphicsSystem* gfx, v2 pos, f32 radius, u32 points);
   Mesh makeMesh_solidCircle(GraphicsSystem* gfx, Circle circle, u32 points);

   // rounds 'points' to the next power of 2
   Mesh makeMesh_outlinedCircle(GraphicsSystem* gfx, v2 pos, f32 radius, u32 points);
   Mesh makeMesh_outlinedCircle(GraphicsSystem* gfx, Circle circle, u32 points);

   Mesh makeMesh_lineSegment(GraphicsSystem* gfx, v2 a, v2 b);
}
