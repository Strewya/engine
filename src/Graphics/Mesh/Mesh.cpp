//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Graphics/Mesh/Mesh.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Graphics/DXRenderer.h>
/******* end headers *******/

namespace Core
{
   Mesh makeSolidQuad(Vec2f pos, Vec2f hs, HTexture texture, HVertexShader vshader, HPixelShader pshader)
   {
      /*
      0-1
      | |
      2-3
      */
      return
      {
         vshader, pshader,
         texture,
         TriangleList,
         //vertices
         {
            //x             y             z  w    r  g  b  a     u   v    h
            {{pos.x - hs.x, pos.y + hs.y, 0, 0}, {1, 1, 1, 1}, {-1, -1}, {0, 0}},
            {{pos.x + hs.x, pos.y + hs.y, 0, 0}, {1, 1, 1, 1}, {-1, -1}, {0, 0}},
            {{pos.x - hs.x, pos.y - hs.y, 0, 0}, {1, 1, 1, 1}, {-1, -1}, {0, 0}},
            {{pos.x + hs.x, pos.y - hs.y, 0, 0}, {1, 1, 1, 1}, {-1, -1}, {0, 0}},
         },
         //indices
         {
            0, 1, 3,
            3, 2, 0
         }
      };
   }
}
