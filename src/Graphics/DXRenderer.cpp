//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Graphics/DXRenderer.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Util/Color.h>
#include <Graphics/Shader/DXShader.h>
#include <Graphics/Texture/DXTexture.h>
#include <Graphics/Vertex.h>
#include <Util/Transform.h>
#include <Util/Utility.h>
#include <Util/Vec2.h>
/******* end headers *******/

namespace Core
{
   static ID3D11Buffer* makeVertexBuffer(ID3D11Device* dev, uint32_t unitSize, uint32_t unitCount);
   static ID3D11Buffer* makeIndexBuffer(ID3D11Device* dev, uint32_t unitSize, uint32_t unitCount);
   static ID3D11Buffer* makeConstantBuffer(ID3D11Device* dev, uint32_t unitSize);
   static ID3D11Buffer* makeInstanceBuffer(ID3D11Device* dev, uint32_t unitSize, uint32_t unitCount);

   bool DXRenderer::init(ID3D11Device* device,
                         ID3D11DeviceContext* deviceContext,
                         ID3D11SamplerState* samplerState)
   {
      CORE_INIT_START(DXRenderer);

      m_dev = device;
      m_devcon = deviceContext;
      m_samplerState = samplerState;
      m_devcon->PSSetSamplers(0, 1, &m_samplerState);

      m_topology = -1;
      m_texture = nullptr;
      m_vertexShader = nullptr;
      m_inputLayout = nullptr;
      m_pixelShader = nullptr;

      CORE_STATUS_AND(m_dev != nullptr);
      CORE_STATUS_AND(m_devcon != nullptr);
      CORE_STATUS_AND(m_samplerState != nullptr);

      D3D11_BLEND_DESC blendDesc;
      ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

      blendDesc.AlphaToCoverageEnable = false;

      D3D11_RENDER_TARGET_BLEND_DESC& rtbd = blendDesc.RenderTarget[0];
      rtbd.BlendEnable = true;
      rtbd.SrcBlend = D3D11_BLEND_SRC_ALPHA;
      rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
      rtbd.BlendOp = D3D11_BLEND_OP_ADD;
      rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
      rtbd.DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
      rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
      rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

      HRESULT hr = m_dev->CreateBlendState(&blendDesc, &m_transparency);

      CORE_INIT_END(DXRenderer);
   }

   bool DXRenderer::shutdown()
   {
      CORE_SHUTDOWN_START(DXRenderer);

      m_devcon = nullptr;
      m_samplerState = nullptr;

      CORE_SHUTDOWN_END(DXRenderer);
   }

   //*****************************************************************
   //          SET PROJECTION
   //*****************************************************************
   void DXRenderer::setProjection(const XMMATRIX& matrix)
   {
      m_camProjection = matrix;
   }

   //*****************************************************************
   //          SET VIEW
   //*****************************************************************
   void DXRenderer::setView(const XMMATRIX& matrix)
   {
      m_camView = matrix;
   }

   //*****************************************************************
   //          SET CULLING
   //*****************************************************************
   void DXRenderer::setCulling(bool isEnabled)
   {
      D3D11_RASTERIZER_DESC rd;
      ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));

      if( isEnabled )
      {
         rd.CullMode = D3D11_CULL_BACK;
      }
      else
      {
         rd.CullMode = D3D11_CULL_NONE;
      }
      rd.FillMode = D3D11_FILL_SOLID;


      ID3D11RasterizerState* state = nullptr;

      HRESULT hr = m_dev->CreateRasterizerState(&rd, &state);
      if( SUCCEEDED(hr) )
      {
         m_devcon->RSSetState(state);
         safeRelease(state);
      }
   }

   //*****************************************************************
   //          SET TEXTURE
   //*****************************************************************
   void DXRenderer::setTexture(const DXTexture& texture)
   {
      if( m_texture != texture.shaderResourceView )
      {
         m_devcon->PSSetShaderResources(0, 1, &texture.shaderResourceView);
         m_texture = texture.shaderResourceView;
      }
   }

   //*****************************************************************
   //          SET VERTEX TOPOLOGY
   //*****************************************************************
   void DXRenderer::setVertexTopology(VertexTopology topology)
   {
      D3D_PRIMITIVE_TOPOLOGY data[] = {
         D3D_PRIMITIVE_TOPOLOGY_LINELIST,
         D3D_PRIMITIVE_TOPOLOGY_LINESTRIP,
         D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
         D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
      };
      if( m_topology != topology )
      {
         auto nextTopology = data[topology];
         m_devcon->IASetPrimitiveTopology(nextTopology);
         m_topology = topology;
      }
   }

   //*****************************************************************
   //          SET SHADER
   //*****************************************************************
   void DXRenderer::setShader(const DXShader& shader)
   {
      // INPUT LAYOUT
      if( m_inputLayout != shader.inputLayout )
      {
         m_devcon->IASetInputLayout(shader.inputLayout);
         m_inputLayout = shader.inputLayout;
      }
      // VERTEX SHADER
      if( m_vertexShader != shader.vertex )
      {
         m_devcon->VSSetShader(shader.vertex, nullptr, 0);
         m_vertexShader = shader.vertex;
      }
      // PIXEL SHADER
      if( m_pixelShader != shader.pixel )
      {
         m_devcon->PSSetShader(shader.pixel, nullptr, 0);
         m_pixelShader = shader.pixel;
      }
   }

   //*****************************************************************
   //          RENDER
   //*****************************************************************
   void DXRenderer::render(Transform transform, Color color, std::vector<Vertex> vertices, std::vector<uint32_t> indices)
   {
      if( vertices.empty() || indices.empty() )
      {
         return;
      }

      for( auto& vx : vertices )
      {
         XMCOLOR fillColor(color.r, color.g, color.b, color.a);
         XMVECTOR fill = XMLoadColor(&fillColor);
         XMVECTOR diffuse = XMLoadFloat4(&vx.diffuse.m_);
         diffuse *= fill;
         XMStoreFloat4(&vx.diffuse.m_, diffuse);

         auto world = XMMatrixIdentity();
         world *= XMMatrixScaling(transform.scale.x, transform.scale.y, 1.0f);
         //m_world *= XMMatrixRotationX(XMConvertToRadians(rotationX));
         //m_world *= XMMatrixRotationY(XMConvertToRadians(rotationY));
         world *= XMMatrixRotationZ(transform.rotation);
         world *= XMMatrixTranslation(transform.position.x, transform.position.y, 0);
         world = world*m_camView*m_camProjection;
         XMVECTOR position = XMLoadFloat4(&vx.position.m_);
         position = XMVector3TransformCoord(position, world);
         XMStoreFloat4(&vx.position.m_, position);
      }


      D3D11_MAPPED_SUBRESOURCE ms;

      auto* vertexBuffer = makeVertexBuffer(m_dev, sizeof(Vertex), vertices.size());
      auto* indexBuffer = makeIndexBuffer(m_dev, sizeof(uint32_t), indices.size());

      HRESULT hr = m_devcon->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
      assert(SUCCEEDED(hr));
      memcpy(ms.pData, vertices.data(), vertices.size() * sizeof(Vertex));
      m_devcon->Unmap(vertexBuffer, 0);

      hr = m_devcon->Map(indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
      assert(SUCCEEDED(hr));
      memcpy(ms.pData, indices.data(), indices.size() * sizeof(uint32_t));
      m_devcon->Unmap(indexBuffer, 0);


      uint32_t stride = sizeof(Vertex);
      uint32_t offset = 0;

      m_devcon->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
      m_devcon->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

      m_devcon->DrawIndexed(indices.size(), 0, 0);

      safeRelease(indexBuffer);
      safeRelease(vertexBuffer);
   }

   //*****************************************************************
   //          MAKE VERTEX BUFFER
   //*****************************************************************
   ID3D11Buffer* makeVertexBuffer(ID3D11Device* dev, uint32_t unitSize, uint32_t unitCount)
   {
      assert(unitSize > 0 && unitCount > 0);

      D3D11_BUFFER_DESC desc;
      ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
      desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
      desc.ByteWidth = unitSize*unitCount;
      desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
      desc.MiscFlags = 0;
      desc.Usage = D3D11_USAGE_DYNAMIC;

      ID3D11Buffer* buffer = nullptr;
      HRESULT hr = dev->CreateBuffer(&desc, nullptr, &buffer);
      assert(SUCCEEDED(hr));
      return buffer;
   }

   //*****************************************************************
   //          MAKE INDEX BUFFER
   //*****************************************************************
   ID3D11Buffer* makeIndexBuffer(ID3D11Device* dev, uint32_t unitSize, uint32_t unitCount)
   {
      assert(unitSize > 0 && unitCount > 0);

      D3D11_BUFFER_DESC desc;
      ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
      desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
      desc.ByteWidth = unitSize*unitCount;
      desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
      desc.MiscFlags = 0;
      desc.Usage = D3D11_USAGE_DYNAMIC;

      ID3D11Buffer* buffer = nullptr;
      HRESULT hr = dev->CreateBuffer(&desc, nullptr, &buffer);
      assert(SUCCEEDED(hr));
      return buffer;
   }

   //*****************************************************************
   //          MAKE CONSTANT BUFFER
   //*****************************************************************
   ID3D11Buffer* makeConstantBuffer(ID3D11Device* dev, uint32_t unitSize)
   {
      assert(unitSize > 0);

      D3D11_BUFFER_DESC desc;

      ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
      desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
      desc.ByteWidth = unitSize;
      desc.CPUAccessFlags = 0;
      desc.MiscFlags = 0;
      desc.Usage = D3D11_USAGE_DEFAULT;

      ID3D11Buffer* buffer = nullptr;
      HRESULT hr = dev->CreateBuffer(&desc, nullptr, &buffer);
      assert(SUCCEEDED(hr));
      return buffer;
   }

   //*****************************************************************
   //          MAKE INSTANCE BUFFER
   //*****************************************************************
   ID3D11Buffer* makeInstanceBuffer(ID3D11Device* dev, uint32_t unitSize, uint32_t unitCount)
   {
      assert(unitSize > 0);

      D3D11_BUFFER_DESC desc;
      ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
      desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
      desc.ByteWidth = unitSize*unitCount;
      desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
      desc.MiscFlags = 0;
      desc.Usage = D3D11_USAGE_DYNAMIC;

      ID3D11Buffer* buffer = nullptr;
      HRESULT hr = dev->CreateBuffer(&desc, nullptr, &buffer);
      assert(SUCCEEDED(hr));
      return buffer;
   }






   void DXRenderer::setData(VertexBuffer vertices, IndexBuffer indices, Transform transform, Color color, float fValue)
   {
      uint32_t vertexOffset = m_verticesToDraw.size();
      for( auto& vx : vertices )
      {
         vx.health.m_.x = fValue;

         XMCOLOR fillColor(color.r, color.g, color.b, color.a);
         XMVECTOR fill = XMLoadColor(&fillColor);
         XMVECTOR diffuse = XMLoadFloat4(&vx.diffuse.m_);
         diffuse *= fill;
         XMStoreFloat4(&vx.diffuse.m_, diffuse);

         auto world = XMMatrixIdentity();
         world *= XMMatrixScaling(transform.scale.x, transform.scale.y, 1.0f);
         //m_world *= XMMatrixRotationX(XMConvertToRadians(rotationX));
         //m_world *= XMMatrixRotationY(XMConvertToRadians(rotationY));
         world *= XMMatrixRotationZ(transform.rotation);
         world *= XMMatrixTranslation(transform.position.x, transform.position.y, 0);
         world = world*m_camView*m_camProjection;
         XMVECTOR position = XMLoadFloat4(&vx.position.m_);
         position = XMVector3TransformCoord(position, world);
         XMStoreFloat4(&vx.position.m_, position);
      }
      m_verticesToDraw.insert(m_verticesToDraw.end(), vertices.begin(), vertices.end());

      for( auto& i : indices )
      {
         i += vertexOffset;
      }
      m_indicesToDraw.insert(m_indicesToDraw.end(), indices.begin(), indices.end());
   }

   void DXRenderer::drawBuffers()
   {
      if( m_verticesToDraw.empty() || m_indicesToDraw.empty() )
         return;

      D3D11_MAPPED_SUBRESOURCE ms;

      auto* vb = makeVertexBuffer(m_dev, sizeof(Vertex), m_verticesToDraw.size());
      auto* ib = makeIndexBuffer(m_dev, sizeof(uint32_t), m_indicesToDraw.size());

      HRESULT hr = m_devcon->Map(vb, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
      assert(SUCCEEDED(hr));
      memcpy(ms.pData, m_verticesToDraw.data(), m_verticesToDraw.size() * sizeof(Vertex));
      m_devcon->Unmap(vb, 0);

      hr = m_devcon->Map(ib, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
      assert(SUCCEEDED(hr));
      memcpy(ms.pData, m_indicesToDraw.data(), m_indicesToDraw.size() * sizeof(uint32_t));
      m_devcon->Unmap(ib, 0);


      uint32_t stride = sizeof(Vertex);
      uint32_t offset = 0;

      m_devcon->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
      m_devcon->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

      m_devcon->DrawIndexed(m_indicesToDraw.size(), 0, 0);

      m_verticesToDraw.clear();
      m_indicesToDraw.clear();

      safeRelease(ib);
      safeRelease(vb);
   }

   IndexBuffer DXRenderer::makeSolidQuadIndices() const
   {
      return
      {
         0, 1, 3,
         3, 2, 0
      };
   }

   VertexBuffer DXRenderer::makeQuadVertices(Vec2f pos, Vec2f hs) const
   {
      /*
      0-1
      | |
      2-3
      */
      return
      {
         {{pos.x - hs.x, pos.y + hs.y, 0}, {1, 1, 1, 1}, {-1, -1}},
         {{pos.x + hs.x, pos.y + hs.y, 0}, {1, 1, 1, 1}, {-1, -1}},
         {{pos.x - hs.x, pos.y - hs.y, 0}, {1, 1, 1, 1}, {-1, -1}},
         {{pos.x + hs.x, pos.y - hs.y, 0}, {1, 1, 1, 1}, {-1, -1}},
      };
   }

   void DXRenderer::setTransparency(bool isEnabled)
   {
      auto transpEnabled = m_transparency;
      if( !isEnabled )
      {
         transpEnabled = nullptr;
      }
      m_devcon->OMSetBlendState(transpEnabled, nullptr, 0xffffffff);
   }
}
