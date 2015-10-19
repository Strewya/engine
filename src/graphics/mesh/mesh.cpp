//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "graphics/mesh/mesh.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "graphics/vertex.h"
#include "graphics/graphics_system.h"
#include "utility/color.h"
#include "utility/utility.h"
#include "utility/geometry/circle.h"
#include "utility/geometry/rect.h"
#include "utility/geometry/vec3.h"
/******* end headers *******/

namespace core
{
   /************************************************************************
    *              QUAD VERTEX ORDER
    *              0--1
    *              |  |
    *              2--3
    ************************************************************************/

   Mesh makeMesh_solidQuad(GraphicsSystem* gfx, v2 pos, v2 hs)
   {
      Mesh result;
      result.topology = TriangleList;
      result.vertices = gfx->allocateVertexBuffer(4);
      result.indices = gfx->allocateIndexBuffer(6);

      auto index = 0U;
      //                            x             y             z    r  g  b  a     u   v    h
      result.vertices.data[index++] = {{pos.x - hs.x, pos.y + hs.y, 0}, {1, 1, 1, 1}, {-1, -1}, 0};
      result.vertices.data[index++] = {{pos.x + hs.x, pos.y + hs.y, 0}, {1, 1, 1, 1}, {-1, -1}, 0};
      result.vertices.data[index++] = {{pos.x - hs.x, pos.y - hs.y, 0}, {1, 1, 1, 1}, {-1, -1}, 0};
      result.vertices.data[index++] = {{pos.x + hs.x, pos.y - hs.y, 0}, {1, 1, 1, 1}, {-1, -1}, 0};

      index = 0U;
      result.indices.data[index++] = 0;
      result.indices.data[index++] = 1;
      result.indices.data[index++] = 3;
      result.indices.data[index++] = 3;
      result.indices.data[index++] = 2;
      result.indices.data[index++] = 0;

      return result;
   }

   Mesh makeMesh_solidQuad(GraphicsSystem* gfx, Rect rect)
   {
      auto result = makeMesh_solidQuad(gfx, rect.center, rect.halfSize);
      return result;
   }

   Mesh makeMesh_outlinedQuad(GraphicsSystem* gfx, v2 pos, v2 hs)
   {
      Mesh result;
      result.topology = LineList;
      result.vertices = gfx->allocateVertexBuffer(4);
      result.indices = gfx->allocateIndexBuffer(8);

      auto index = 0U;
      //                            x             y             z    r  g  b  a     u   v    h
      result.vertices.data[index++] = {{pos.x - hs.x, pos.y + hs.y, 0}, {1, 1, 1, 1}, {-1, -1}, 0};
      result.vertices.data[index++] = {{pos.x + hs.x, pos.y + hs.y, 0}, {1, 1, 1, 1}, {-1, -1}, 0};
      result.vertices.data[index++] = {{pos.x - hs.x, pos.y - hs.y, 0}, {1, 1, 1, 1}, {-1, -1}, 0};
      result.vertices.data[index++] = {{pos.x + hs.x, pos.y - hs.y, 0}, {1, 1, 1, 1}, {-1, -1}, 0};

      index = 0U;
      result.indices.data[index++] = 0;
      result.indices.data[index++] = 1;
      result.indices.data[index++] = 1;
      result.indices.data[index++] = 3;
      result.indices.data[index++] = 3;
      result.indices.data[index++] = 2;
      result.indices.data[index++] = 2;
      result.indices.data[index++] = 0;

      return result;
   }

   Mesh makeMesh_outlinedQuad(GraphicsSystem* gfx, Rect rect)
   {
      auto result = makeMesh_outlinedQuad(gfx, rect.center, rect.halfSize);
      return result;
   }

   Mesh makeMesh_texturedQuad(GraphicsSystem* gfx, v2 pos, v2 hs, v2 topLeftUV, v2 botRightUV)
   {
      Mesh result;
      result.topology = TriangleList;
      result.vertices = gfx->allocateVertexBuffer(4);
      result.indices = gfx->allocateIndexBuffer(6);

      auto index = 0U;
      //                            x             y             z    r  g  b  a     u   v    h
      result.vertices.data[index++] = {{pos.x - hs.x, pos.y + hs.y, 0}, {1, 1, 1, 1}, {topLeftUV.x, topLeftUV.y}, 0};
      result.vertices.data[index++] = {{pos.x + hs.x, pos.y + hs.y, 0}, {1, 1, 1, 1}, {botRightUV.x, topLeftUV.y}, 0};
      result.vertices.data[index++] = {{pos.x - hs.x, pos.y - hs.y, 0}, {1, 1, 1, 1}, {topLeftUV.x, botRightUV.y}, 0};
      result.vertices.data[index++] = {{pos.x + hs.x, pos.y - hs.y, 0}, {1, 1, 1, 1}, {botRightUV.x, botRightUV.y}, 0};

      index = 0U;
      result.indices.data[index++] = 0;
      result.indices.data[index++] = 1;
      result.indices.data[index++] = 3;
      result.indices.data[index++] = 3;
      result.indices.data[index++] = 2;
      result.indices.data[index++] = 0;

      return result;
   }

   Mesh makeMesh_texturedQuad(GraphicsSystem* gfx, Rect rect, v2 topLeftUV, v2 botRightUV)
   {
      auto result = makeMesh_texturedQuad(gfx, rect.center, rect.halfSize, topLeftUV, botRightUV);
      return result;
   }

   Mesh makeMesh_solidCircle(GraphicsSystem* gfx, v2 pos, f32 radius, u32 points)
   {
      //round to nearest largest power of two
      --points;
      points |= points >> 1;
      points |= points >> 2;
      points |= points >> 4;
      points |= points >> 8;
      points |= points >> 16;
      ++points;

      Mesh result;
      result.topology = TriangleList;
      result.vertices = gfx->allocateVertexBuffer(points + 1);
      result.indices = gfx->allocateIndexBuffer(points * 3);

      auto index = 0U;
      f32 degreeOffset = 360.0f / points;
      result.vertices.data[index++] = {{pos.x, pos.y, 0}, {1, 1, 1, 1}, {-1, -1}, 0};
      for( u32 i = 0; i < points; ++i )
      {
         result.vertices.data[index++] = {{pos.x + radius*cos(Deg2Rad(i*degreeOffset)), pos.y + radius*sin(Deg2Rad(i*degreeOffset)), 0}, {1, 1, 1, 1}, {-1, -1}, 0};
      }
      index = 0U;
      for( u32 i = 1; i <= points; ++i )
      {
         result.indices.data[index++] = i;
         result.indices.data[index++] = 0;
         result.indices.data[index++] = (i % points) + 1;
      }

      return result;
   }

   Mesh makeMesh_solidCircle(GraphicsSystem* gfx, Circle circle, u32 points)
   {
      auto result = makeMesh_solidCircle(gfx, circle.center, circle.radius, points);
      return result;
   }

   Mesh makeOutlineCircle(GraphicsSystem* gfx, v2 pos, f32 radius, u32 points)
   {
      //round to nearest largest power of two
      --points;
      points |= points >> 1;
      points |= points >> 2;
      points |= points >> 4;
      points |= points >> 8;
      points |= points >> 16;
      ++points;

      Mesh result;
      result.topology = LineStrip;
      result.vertices = gfx->allocateVertexBuffer(points);
      result.indices = gfx->allocateIndexBuffer(points + 1);

      auto index = 0U;

      f32 degreeOffset = 360.0f / points;
      for( u32 i = 0; i < points; ++i )
      {
         result.vertices.data[index++] = {{pos.x + radius*cos(Deg2Rad(i*degreeOffset)), pos.y + radius*sin(Deg2Rad(i*degreeOffset)), 0}, {1, 1, 1, 1}, {-1, -1}, 0};
      }
      index = 0U;
      for( u32 i = 0; i < points; ++i )
      {
         result.indices.data[index++] = i;
      }
      result.indices.data[index++] = 0;

      return result;
   }

   Mesh makeOutlineCircle(GraphicsSystem* gfx, Circle circle, u32 points)
   {
      auto result = makeOutlineCircle(gfx, circle.center, circle.radius, points);
      return result;
   }

   Mesh makeLine(GraphicsSystem* gfx, v2 a, v2 b)
   {
      Mesh result{};
      result.topology = LineList;
      result.indices = gfx->allocateIndexBuffer(2);
      result.vertices = gfx->allocateVertexBuffer(2);
      
      auto index = 0U;
      result.vertices.data[index++] = {{a.x, a.y, 0}, {1, 1, 1, 1}, {-1, -1}, 0};
      result.vertices.data[index++] = {{b.x, b.y, 0}, {1, 1, 1, 1}, {-1, -1}, 0};
      
      index = 0U;
      result.indices.data[index++] = 0;
      result.indices.data[index++] = 1;

      return result;
   }
}
