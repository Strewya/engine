//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Graphics/DXRenderer.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
   bool DXRenderer::init(ID3D11DeviceContext* deviceContext,
                         ID3D11SamplerState* samplerState)
   {
      m_devcon = deviceContext;
      m_samplerState = samplerState;

      CORE_STATUS(m_devcon != nullptr);
      CORE_STATUS_AND(m_samplerState != nullptr);
      CORE_INIT(DXRenderer);
   }

   bool DXRenderer::shutdown()
   {
      m_devcon = nullptr;
      m_samplerState = nullptr;

      CORE_STATUS(true);
      CORE_SHUTDOWN(DXRenderer);
   }

   //*****************************************************************
   //          SET TEXTURE
   //*****************************************************************
   void DXRenderer::setTexture(ID3D11ShaderResourceView* texture)
   {
      m_devcon->PSSetSamplers(0, 1, &m_samplerState);
      m_devcon->PSSetShaderResources(0, 1, &texture);
   }

   //*****************************************************************
   //          SET VERTEX TOPOLOGY
   //*****************************************************************
   void DXRenderer::setVertexTopology(D3D_PRIMITIVE_TOPOLOGY topology)
   {
      m_devcon->IASetPrimitiveTopology(topology);
   }
}
