//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Graphics/Vertex.h>
/******* C++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace Core
{
   VertexPosition::VertexPosition(float x, float y, float z)
      : m_{x, y, z, 0}
   {
   }

   void VertexPosition::set(float x, float y, float z)
   {
      m_.x = x;
      m_.y = y;
      m_.z = z;
      m_.w = 0;
   }

   D3D11_INPUT_ELEMENT_DESC VertexPosition::getElementDesc(uint32_t offset)
   {
      /*
      SemanticName, SemanticIndex, Format, InputSlot, AlignedByteOffset, InputSlotClass, InstanceDataStepRate
      */
      return{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offset, D3D11_INPUT_PER_VERTEX_DATA, 0};
   }

   VertexDiffuse::VertexDiffuse(float r, float g, float b, float a)
      : m_{r, g, b, a}
   {
   }

   void VertexDiffuse::set(float r, float g, float b, float a)
   {
      m_.x = r;
      m_.y = g;
      m_.z = b;
      m_.w = a;
   }

   D3D11_INPUT_ELEMENT_DESC VertexDiffuse::getElementDesc(uint32_t offset)
   {
      /*
      SemanticName, SemanticIndex, Format, InputSlot, AlignedByteOffset, InputSlotClass, InstanceDataStepRate
      */
      return{"DIFFUSE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offset, D3D11_INPUT_PER_VERTEX_DATA, 0};
   }

   VertexTextureUV::VertexTextureUV(float u, float v)
      : m_{u, v}
   {
   }

   void VertexTextureUV::set(float u, float v)
   {
      m_.x = u;
      m_.y = v;
   }

   D3D11_INPUT_ELEMENT_DESC VertexTextureUV::getElementDesc(uint32_t offset)
   {
      /*
      SemanticName, SemanticIndex, Format, InputSlot, AlignedByteOffset, InputSlotClass, InstanceDataStepRate
      */
      return{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offset, D3D11_INPUT_PER_VERTEX_DATA, 0};
   }

   //////////////// DEFAULT VERTEX

   std::vector<D3D11_INPUT_ELEMENT_DESC> DefaultVertex::getDescription()
   {
      return
      {
         VertexPosition::getElementDesc(0),
         VertexDiffuse::getElementDesc(D3D11_APPEND_ALIGNED_ELEMENT),
         VertexTextureUV::getElementDesc(D3D11_APPEND_ALIGNED_ELEMENT)
      };
   }

   //////////// VERTEX

   VertexHealthValue::VertexHealthValue(float h)
      : m_(h,h)
   {
   }

   void VertexHealthValue::set(float health)
   {
      m_.x = health;
   }

   D3D11_INPUT_ELEMENT_DESC VertexHealthValue::getElementDesc(uint32_t offset)
   {
      return{"HEALTH", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offset, D3D11_INPUT_PER_VERTEX_DATA, 0};
   }

   std::vector<D3D11_INPUT_ELEMENT_DESC> Vertex::getDescription()
   {
      return
      {
         VertexPosition::getElementDesc(0),
         VertexDiffuse::getElementDesc(D3D11_APPEND_ALIGNED_ELEMENT),
         VertexTextureUV::getElementDesc(D3D11_APPEND_ALIGNED_ELEMENT),
         VertexHealthValue::getElementDesc(D3D11_APPEND_ALIGNED_ELEMENT),
         /*
         {"WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},
         {"WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
         {"WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
         {"WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
         {"FILL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
         */
      };
   }
}