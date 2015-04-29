//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "graphics/mesh/mesh.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "graphics/dx_renderer.h"
#include "util/color.h"
#include "util/vec2.h"
#include "util/vec3.h"
/******* end headers *******/

namespace Core
{
   Mesh makeSolidQuad(Vec2f pos, Vec2f hs, HVertexShader vshader, HPixelShader pshader)
   {
      /*
      0-1
      | |
      2-3
      */
      return
      {
         vshader, pshader,
         HTexture{},
         TriangleList,
         //vertices
         {
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
   }

   Mesh makeTexturedQuad(Vec2f pos, Vec2f hs, HTexture texture, Vec2f topLeftUV, Vec2f botRightUV, HVertexShader vshader, HPixelShader pshader)
   {
      /*
      0-1
      | |
      2-3
      */
      Mesh mesh
      {
         vshader, pshader,
         texture,
         TriangleList,
         //vertices
         {
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

}
