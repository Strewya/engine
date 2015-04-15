#pragma once
/********************************************
*  contents:   Mesh
*  usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <Graphics/Shader/ShaderHandle.h>
#include <Graphics/Texture/DXTextureHandle.h>
#include <Graphics/Typedefs.h>
#include <Util/Vec2Fwd.h>
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
