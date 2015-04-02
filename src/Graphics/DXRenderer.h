#pragma once
/********************************************
*  contents:   DXRenderer
*  usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
#include <Graphics/DXInclude.h>
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   enum VertexTopology
   {
      LineList,
      LineStrip,
      TriangleList,
      TriangleStrip,
   };

   class DXRenderer
   {
   public:
      bool init(ID3D11DeviceContext* deviceContext, ID3D11SamplerState* samplerState);
      bool shutdown();

      void setTexture(ID3D11ShaderResourceView* texture);
      void setVertexTopology(D3D_PRIMITIVE_TOPOLOGY topology);

   private:
      ID3D11DeviceContext* m_devcon;
      ID3D11SamplerState* m_samplerState;
   };
}
