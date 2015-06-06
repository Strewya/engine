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
/******* end header inclusion *******/

namespace core
{
   struct Circle;
   struct Rect;
   struct Vec2;

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
   
   Mesh makeSolidQuad(Vec2 pos, Vec2 hs, HVertexShader vshader, HPixelShader pshader);
   Mesh makeSolidQuad(Rect rect, HVertexShader vshader, HPixelShader pshader);
   Mesh makeOutlineQuad(Vec2 pos, Vec2 hs, HVertexShader vshader, HPixelShader pshader);
   Mesh makeOutlineQuad(Rect rect, HVertexShader vshader, HPixelShader pshader);
   Mesh makeTexturedQuad(Vec2 pos, Vec2 hs, HTexture texture, Vec2 topLeftUV, Vec2 botRightUV, HVertexShader vshader, HPixelShader pshader);
   Mesh makeTexturedQuad(Rect rect, HTexture texture, Vec2 topLeftUV, Vec2 botRightUV, HVertexShader vshader, HPixelShader pshader);
   // rounds 'points' to the next power of 2
   Mesh makeSolidCircle(Vec2 pos, float radius, uint32_t points, HVertexShader vshader, HPixelShader pshader);
   Mesh makeSolidCircle(Circle circle, uint32_t points, HVertexShader vshader, HPixelShader pshader);
   // rounds 'points' to the next power of 2
   Mesh makeOutlineCircle(Vec2 pos, float radius, uint32_t points, HVertexShader vshader, HPixelShader pshader);
   Mesh makeOutlineCircle(Circle circle, uint32_t points, HVertexShader vshader, HPixelShader pshader);
}
