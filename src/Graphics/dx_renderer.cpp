//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "graphics/dx_renderer.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "util/color.h"
#include "graphics/shader/vertex/vertex_shader.h"
#include "graphics/shader/pixel/pixel_shader.h"
#include "graphics/texture/texture.h"
#include "graphics/vertex.h"
#include "util/transform.h"
#include "util/utility.h"
#include "util/geometry/vec2.h"
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

      m_topology = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
      m_texture = nullptr;
      m_inputLayout = nullptr;
      m_vertexShader = nullptr;
      m_pixelShader = nullptr;
      m_rasterizerState = nullptr;
      m_blendState = nullptr;

      CORE_STATUS_AND(m_dev != nullptr);
      CORE_STATUS_AND(m_devcon != nullptr);
      CORE_STATUS_AND(m_samplerState != nullptr);

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
   void DXRenderer::setBlendState(ID3D11BlendState* blendState)
   {
      if( m_blendState != blendState )
      {
         m_devcon->OMSetBlendState(blendState, nullptr, 0xffffffff);
         m_blendState = blendState;
      }
   }

   //*****************************************************************
   //          SET TRANSPARENCY
   //*****************************************************************
   void DXRenderer::setRasterizerState(ID3D11RasterizerState* rasterizerState)
   {
      if( m_rasterizerState != rasterizerState )
      {
         m_devcon->RSSetState(rasterizerState);
         m_rasterizerState = rasterizerState;
      }
   }

   //*****************************************************************
   //          SET TEXTURE
   //*****************************************************************
   void DXRenderer::setTexture(ID3D11ShaderResourceView* shaderResourceView)
   {
      if( m_texture != shaderResourceView )
      {
         m_devcon->PSSetShaderResources(0, 1, &shaderResourceView);
         m_texture = shaderResourceView;
      }
   }

   //*****************************************************************
   //          SET VERTEX TOPOLOGY
   //*****************************************************************
   void DXRenderer::setVertexTopology(D3D_PRIMITIVE_TOPOLOGY topology)
   {
      if( m_topology != topology )
      {
         m_devcon->IASetPrimitiveTopology(topology);
         m_topology = topology;
      }
   }

   //*****************************************************************
   //          SET INPUT LAYOUT
   //*****************************************************************
   void DXRenderer::setInputLayout(ID3D11InputLayout* layout)
   {
      if( m_inputLayout != layout )
      {
         m_devcon->IASetInputLayout(layout);
         m_inputLayout = layout;
      }
   }

   //*****************************************************************
   //          SET SHADER - VERTEX
   //*****************************************************************
   void DXRenderer::setShader(ID3D11VertexShader* shader)
   {
      if( m_vertexShader != shader)
      {
         m_devcon->VSSetShader(shader, nullptr, 0);
         m_vertexShader = shader;
      }
   }

   //*****************************************************************
   //          SET SHADER - PIXEL
   //*****************************************************************
   void DXRenderer::setShader(ID3D11PixelShader* shader)
   {
      if( m_pixelShader != shader)
      {
         m_devcon->PSSetShader(shader, nullptr, 0);
         m_pixelShader = shader;
      }
   }

   //*****************************************************************
   //          RENDER
   //*****************************************************************
   void DXRenderer::render(Transform transform, Color color, HealthVertexBuffer vertices, IndexBuffer indices)
   {
      if( vertices.empty() || indices.empty() )
      {
         return;
      }

      for( auto& vx : vertices )
      {
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

      auto* vertexBuffer = makeVertexBuffer(m_dev, sizeof(HealthVertex), vertices.size());
      auto* indexBuffer = makeIndexBuffer(m_dev, sizeof(uint32_t), indices.size());

      HRESULT hr = m_devcon->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
      assert(SUCCEEDED(hr));
      memcpy(ms.pData, vertices.data(), vertices.size() * sizeof(HealthVertex));
      m_devcon->Unmap(vertexBuffer, 0);

      hr = m_devcon->Map(indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
      assert(SUCCEEDED(hr));
      memcpy(ms.pData, indices.data(), indices.size() * sizeof(uint32_t));
      m_devcon->Unmap(indexBuffer, 0);


      uint32_t stride = sizeof(HealthVertex);
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
}
