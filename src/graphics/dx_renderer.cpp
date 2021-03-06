//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "graphics/dx_renderer.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "utility/color.h"
#include "graphics/shader/vertex/vertex_shader.h"
#include "graphics/shader/pixel/pixel_shader.h"
#include "graphics/texture/texture.h"
#include "graphics/vertex.h"
#include "utility/transform.h"
#include "utility/utility.h"
/******* end headers *******/

namespace core
{
   core_internal ID3D11Buffer* makeVertexBuffer(ID3D11Device* dev, u32 unitSize, u32 unitCount);
   core_internal ID3D11Buffer* makeIndexBuffer(ID3D11Device* dev, u32 unitSize, u32 unitCount);
   core_internal ID3D11Buffer* makeConstantBuffer(ID3D11Device* dev, u32 unitSize);
   core_internal ID3D11Buffer* makeInstanceBuffer(ID3D11Device* dev, u32 unitSize, u32 unitCount);

   void DXRenderer::init(ID3D11Device* device,
                         ID3D11DeviceContext* deviceContext,
                         ID3D11SamplerState* samplerState)
   {
      m_dev = device;
      m_devcon = deviceContext;
      m_samplerState = samplerState;
      m_devcon->PSSetSamplers(0, 1, &m_samplerState);

      m_topology = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
      m_texture = nullptr;
      m_inputLayout = nullptr;
      m_vertexShader = nullptr;
      m_pixelShader = nullptr;
      m_rasterizerState = nullptr;
      m_blendState = nullptr;

      CORE_ASSERT_DBGERR(m_dev != nullptr, "D3D11 device is NULL in DXRenderer!");
      CORE_ASSERT_DBGERR(m_devcon != nullptr, "D3D11 device context is NULL in DXRenderer!");
      CORE_ASSERT_DBGERR(m_samplerState != nullptr, "D3D11 sampler state is NULL in DXRenderer!");
   }

   void DXRenderer::shutdown()
   {
      m_devcon = nullptr;
      m_samplerState = nullptr;
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

   const XMMATRIX& DXRenderer::getProjection() const
   {
      return m_camProjection;
   }

   const XMMATRIX& DXRenderer::getView() const
   {
      return m_camView;
   }

   //*****************************************************************
   //          SET CULLING
   //*****************************************************************
   void DXRenderer::setBlendState(ID3D11BlendState* blendState)
   {
      if( m_blendState != blendState )
      {
         m_blendState = blendState;
         m_devcon->OMSetBlendState(blendState, nullptr, 0xffffffff);
      }
   }

   //*****************************************************************
   //          SET TRANSPARENCY
   //*****************************************************************
   void DXRenderer::setRasterizerState(ID3D11RasterizerState* rasterizerState)
   {
      if( m_rasterizerState != rasterizerState )
      {
         m_rasterizerState = rasterizerState;
         m_devcon->RSSetState(rasterizerState);
      }
   }

   //*****************************************************************
   //          SET TEXTURE
   //*****************************************************************
   void DXRenderer::setTexture(ID3D11ShaderResourceView* shaderResourceView)
   {
      if( m_texture != shaderResourceView )
      {
         m_texture = shaderResourceView;
         m_devcon->PSSetShaderResources(0, 1, &shaderResourceView);
      }
   }

   //*****************************************************************
   //          SET VERTEX TOPOLOGY
   //*****************************************************************
   void DXRenderer::setVertexTopology(D3D_PRIMITIVE_TOPOLOGY topology)
   {
      if( m_topology != topology )
      {
         m_topology = topology;
         m_devcon->IASetPrimitiveTopology(topology);
      }
   }

   //*****************************************************************
   //          SET INPUT LAYOUT
   //*****************************************************************
   void DXRenderer::setInputLayout(ID3D11InputLayout* layout)
   {
      if( m_inputLayout != layout )
      {
         m_inputLayout = layout;
         m_devcon->IASetInputLayout(layout);
      }
   }

   //*****************************************************************
   //          SET SHADER - VERTEX
   //*****************************************************************
   void DXRenderer::setShader(ID3D11VertexShader* shader)
   {
      if( m_vertexShader != shader )
      {
         m_vertexShader = shader;
         m_devcon->VSSetShader(shader, nullptr, 0);
      }
   }

   //*****************************************************************
   //          SET SHADER - PIXEL
   //*****************************************************************
   void DXRenderer::setShader(ID3D11PixelShader* shader)
   {
      if( m_pixelShader != shader )
      {
         m_pixelShader = shader;
         m_devcon->PSSetShader(shader, nullptr, 0);
      }
   }

   //*****************************************************************
   //          RENDER
   //*****************************************************************
   void DXRenderer::render(Transform transform, Color color, HealthVertexBuffer vertices, IndexBuffer indices)
   {
      if( vertices.size == 0 || indices.size == 0 )
      {
         return;
      }

      for( auto i = 0U; i < vertices.size; ++i )
      {
         auto& vx = vertices.data[i];
         XMCOLOR fillColor(color.r, color.g, color.b, color.a);
         XMVECTOR fill = XMLoadColor(&fillColor);
         XMVECTOR diffuse = XMLoadFloat4(&vx.diffuse);
         diffuse *= fill;
         XMStoreFloat4(&vx.diffuse, diffuse);

         auto world = XMMatrixIdentity();
         world *= XMMatrixScaling(transform.scale.x, transform.scale.y, 1.0f);
         //m_world *= XMMatrixRotationX(XMConvertToRadians(rotationX));
         //m_world *= XMMatrixRotationY(XMConvertToRadians(rotationY));
         world *= XMMatrixRotationZ(transform.rotation);
         world *= XMMatrixTranslation(transform.position.x, transform.position.y, 0);
         world = world*m_camView*m_camProjection;
         XMVECTOR position = XMLoadFloat4(&vx.position);
         position = XMVector3TransformCoord(position, world);
         XMStoreFloat4(&vx.position, position);
      }

      D3D11_MAPPED_SUBRESOURCE ms;

      auto* vertexBuffer = makeVertexBuffer(m_dev, sizeof(HealthVertex), vertices.size);
      auto* indexBuffer = makeIndexBuffer(m_dev, sizeof(u32), indices.size);

      HRESULT hr = m_devcon->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
      CORE_ASSERT_DBGERR(SUCCEEDED(hr), "D3D could not map vertex buffer for writing");
      memcpy(ms.pData, vertices.data, vertices.size * sizeof(HealthVertex));
      m_devcon->Unmap(vertexBuffer, 0);

      hr = m_devcon->Map(indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
      CORE_ASSERT_DBGERR(SUCCEEDED(hr), "D3D could not map index buffer for writing");
      memcpy(ms.pData, indices.data, indices.size * sizeof(u32));
      m_devcon->Unmap(indexBuffer, 0);


      u32 stride = sizeof(HealthVertex);
      u32 offset = 0;

      m_devcon->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
      m_devcon->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

      m_devcon->DrawIndexed(indices.size, 0, 0);

      safeRelease(indexBuffer);
      safeRelease(vertexBuffer);
   }

   //*****************************************************************
   //          MAKE VERTEX BUFFER
   //*****************************************************************
   ID3D11Buffer* makeVertexBuffer(ID3D11Device* dev, u32 unitSize, u32 unitCount)
   {
      CORE_ASSERT_DBGERR(unitSize > 0 && unitCount > 0, "Attempting to create vertex buffer with unit size or unit count 0");

      D3D11_BUFFER_DESC desc;
      ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
      desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
      desc.ByteWidth = unitSize*unitCount;
      desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
      desc.MiscFlags = 0;
      desc.Usage = D3D11_USAGE_DYNAMIC;

      ID3D11Buffer* buffer = nullptr;
      HRESULT hr = dev->CreateBuffer(&desc, nullptr, &buffer);
      CORE_ASSERT_DBGERR(SUCCEEDED(hr), "D3D could not create vertex buffer");
      return buffer;
   }

   //*****************************************************************
   //          MAKE INDEX BUFFER
   //*****************************************************************
   ID3D11Buffer* makeIndexBuffer(ID3D11Device* dev, u32 unitSize, u32 unitCount)
   {
      CORE_ASSERT_DBGERR(unitSize > 0 && unitCount > 0, "Attempting to create index buffer with unit size 0");

      D3D11_BUFFER_DESC desc;
      ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
      desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
      desc.ByteWidth = unitSize*unitCount;
      desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
      desc.MiscFlags = 0;
      desc.Usage = D3D11_USAGE_DYNAMIC;

      ID3D11Buffer* buffer = nullptr;
      HRESULT hr = dev->CreateBuffer(&desc, nullptr, &buffer);
      CORE_ASSERT_DBGERR(SUCCEEDED(hr), "D3D could not create index buffer");
      return buffer;
   }

   //*****************************************************************
   //          MAKE CONSTANT BUFFER
   //*****************************************************************
   ID3D11Buffer* makeConstantBuffer(ID3D11Device* dev, u32 unitSize)
   {
      CORE_ASSERT_DBGERR(unitSize > 0, "Attempting to create constant buffer with unit size 0");

      D3D11_BUFFER_DESC desc;

      ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
      desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
      desc.ByteWidth = unitSize;
      desc.CPUAccessFlags = 0;
      desc.MiscFlags = 0;
      desc.Usage = D3D11_USAGE_DEFAULT;

      ID3D11Buffer* buffer = nullptr;
      HRESULT hr = dev->CreateBuffer(&desc, nullptr, &buffer);
      CORE_ASSERT_DBGERR(SUCCEEDED(hr), "D3D could not create constant buffer");
      return buffer;
   }

   //*****************************************************************
   //          MAKE INSTANCE BUFFER
   //*****************************************************************
   ID3D11Buffer* makeInstanceBuffer(ID3D11Device* dev, u32 unitSize, u32 unitCount)
   {
      CORE_ASSERT_DBGERR(unitSize > 0, "Attempting to create instance buffer with unit size 0");

      D3D11_BUFFER_DESC desc;
      ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
      desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
      desc.ByteWidth = unitSize*unitCount;
      desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
      desc.MiscFlags = 0;
      desc.Usage = D3D11_USAGE_DYNAMIC;

      ID3D11Buffer* buffer = nullptr;
      HRESULT hr = dev->CreateBuffer(&desc, nullptr, &buffer);
      CORE_ASSERT_DBGERR(SUCCEEDED(hr), "D3D could not create instance buffer");
      return buffer;
   }
}
