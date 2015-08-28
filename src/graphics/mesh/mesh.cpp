//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "graphics/mesh/mesh.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "graphics/vertex.h"
#include "utility/color.h"
#include "utility/utility.h"
#include "utility/geometry/circle.h"
#include "utility/geometry/rect.h"
#include "utility/geometry/vec3.h"
/******* end headers *******/

namespace core
{
   Mesh makeSolidQuad(Rect rect, HVertexShader vshader, HPixelShader pshader)
   {
      auto result = makeSolidQuad(rect.center, rect.halfSize, vshader, pshader);
      return result;
   }

   Mesh makeSolidQuad(Vec2 pos, Vec2 hs, HVertexShader vshader, HPixelShader pshader)
   {
      Mesh result
      {
         vshader, pshader,
         HTexture{},
         TriangleList,
         //vertices
         {
            /*
            0--1
            |  |
            2--3
            */
            //x             y             z    r  g  b  a     u   v    h
            {{pos.x - hs.x, pos.y + hs.y, 0}, {1, 1, 1, 1}, {-1, -1}, 0},
            {{pos.x + hs.x, pos.y + hs.y, 0}, {1, 1, 1, 1}, {-1, -1}, 0},
            {{pos.x - hs.x, pos.y - hs.y, 0}, {1, 1, 1, 1}, {-1, -1}, 0},
            {{pos.x + hs.x, pos.y - hs.y, 0}, {1, 1, 1, 1}, {-1, -1}, 0},
         },
         //indices
         {
            0, 1, 3,
            3, 2, 0
         }
      };
      return result;
   }

   Mesh makeOutlineQuad(Rect rect, HVertexShader vshader, HPixelShader pshader)
   {
      auto result = makeOutlineQuad(rect.center, rect.halfSize, vshader, pshader);
      return result;
   }

   Mesh makeOutlineQuad(Vec2 pos, Vec2 hs, HVertexShader vshader, HPixelShader pshader)
   {
      Mesh result
      {
         vshader, pshader,
         HTexture{},
         LineList,
         //vertices
         {
            /*
            0--1
            |  |
            2--3
            */
            //x             y             z    r  g  b  a     u   v    h
            {{pos.x - hs.x, pos.y + hs.y, 0}, {1, 1, 1, 1}, {-1, -1}, 0},
            {{pos.x + hs.x, pos.y + hs.y, 0}, {1, 1, 1, 1}, {-1, -1}, 0},
            {{pos.x - hs.x, pos.y - hs.y, 0}, {1, 1, 1, 1}, {-1, -1}, 0},
            {{pos.x + hs.x, pos.y - hs.y, 0}, {1, 1, 1, 1}, {-1, -1}, 0},
         },
         //indices
         {
            0, 1,
            1, 3,
            3, 2,
            2, 0,
         }
      };
      return result;
   }

   Mesh makeTexturedQuad(Rect rect, HTexture texture, Vec2 topLeftUV, Vec2 botRightUV, HVertexShader vshader, HPixelShader pshader)
   {
      auto result = makeTexturedQuad(rect.center, rect.halfSize, texture, topLeftUV, botRightUV, vshader, pshader);
      return result;
   }

   Mesh makeTexturedQuad(Vec2 pos, Vec2 hs, HTexture texture, Vec2 topLeftUV, Vec2 botRightUV, HVertexShader vshader, HPixelShader pshader)
   {
      Mesh mesh
      {
         vshader, pshader,
         texture,
         TriangleList,
         //vertices
         {
            /*
            0--1
            |  |
            2--3
            */
            //x             y             z    r  g  b  a     u   v                      h
            {{pos.x - hs.x, pos.y + hs.y, 0}, {1, 1, 1, 1}, {topLeftUV.x, topLeftUV.y}, 0},
            {{pos.x + hs.x, pos.y + hs.y, 0}, {1, 1, 1, 1}, {botRightUV.x, topLeftUV.y}, 0},
            {{pos.x - hs.x, pos.y - hs.y, 0}, {1, 1, 1, 1}, {topLeftUV.x, botRightUV.y}, 0},
            {{pos.x + hs.x, pos.y - hs.y, 0}, {1, 1, 1, 1}, {botRightUV.x, botRightUV.y}, 0},
         },
         //indices
         {
            0, 1, 3,
            3, 2, 0
         }
      };
      return mesh;
   }

   Mesh makeSolidCircle(Circle circle, uint32_t points, HVertexShader vshader, HPixelShader pshader)
   {
      auto result = makeSolidCircle(circle.center, circle.radius, points, vshader, pshader);
      return result;
   }

   Mesh makeSolidCircle(Vec2 pos, float radius, uint32_t points, HVertexShader vshader, HPixelShader pshader)
   {
      --points;
      points |= points >> 1;
      points |= points >> 2;
      points |= points >> 4;
      points |= points >> 8;
      points |= points >> 16;
      ++points;

      Mesh result;
      result.vshader = vshader;
      result.pshader = pshader;
      result.vertices.reserve(points+1);
      float degreeOffset = 360.0f / points;
      result.vertices.push_back({{pos.x, pos.y, 0}, {1, 1, 1, 1}, {-1, -1}, 0});
      for( uint32_t i = 0; i < points; ++i )
      {
         result.vertices.push_back({{pos.x + radius*cos(Deg2Rad(i*degreeOffset)), pos.y + radius*sin(Deg2Rad(i*degreeOffset)), 0}, {1, 1, 1, 1}, {-1, -1}, 0});
      }
      result.indices.reserve(points * 3);
      for( uint32_t i = 1; i <= points; ++i )
      {
         result.indices.push_back(i);
         result.indices.push_back(0);
         result.indices.push_back((i % points) + 1);
      }
      result.topology = TriangleList;

      return result;
   }

   Mesh makeOutlineCircle(Circle circle, uint32_t points, HVertexShader vshader, HPixelShader pshader)
   {
      auto result = makeOutlineCircle(circle.center, circle.radius, points, vshader, pshader);
      return result;
   }

   Mesh makeOutlineCircle(Vec2 pos, float radius, uint32_t points, HVertexShader vshader, HPixelShader pshader)
   {
      --points;
      points |= points >> 1;
      points |= points >> 2;
      points |= points >> 4;
      points |= points >> 8;
      points |= points >> 16;
      ++points;

      Mesh result;
      result.vshader = vshader;
      result.pshader = pshader;
      result.vertices.reserve(points);
      float degreeOffset = 360.0f / points;
      for( uint32_t i = 0; i < points; ++i )
      {
         result.vertices.push_back({{pos.x + radius*cos(Deg2Rad(i*degreeOffset)), pos.y + radius*sin(Deg2Rad(i*degreeOffset)), 0}, {1, 1, 1, 1}, {-1, -1}, 0});
      }
      result.indices.reserve(points + 1);
      for( uint32_t i = 0; i < points; ++i )
      {
         result.indices.push_back(i);
      }
      result.indices.push_back(0);
      result.topology = LineStrip;

      return result;
   }

   Mesh makeLine(Vec2 a, Vec2 b, HVertexShader vshader, HPixelShader pshader)
   {
      Mesh result{};
      result.pshader = pshader;
      result.vshader = vshader;
      result.topology = LineList;
      result.indices = {0, 1};
      result.vertices = {
         {{a.x, a.y, 0}, {1, 1, 1, 1}, {-1, -1}, 0},
         {{b.x, b.y, 0}, {1, 1, 1, 1}, {-1, -1}, 0},
      };
      return result;
   }
}
