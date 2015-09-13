//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "graphics/vertex.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "utility/color.h"
#include "utility/utility.h"
#include "utility/memory.h"
/******* end headers *******/

namespace core
{
   //////////////// DEFAULT VERTEX

   DefaultVertex::DefaultVertex()
      : DefaultVertex({}, {}, {-1, -1})
   {
   }

   DefaultVertex::DefaultVertex(v3 position, Color color, v2 textureUV)
      : position(position.x, position.y, position.z, 0),
      diffuse(color.r, color.g, color.b, color.a),
      textureUV(textureUV.x, textureUV.y)
   {
   }

   void DefaultVertex::setPosition(v3 pos)
   {
      setPosition(pos.x, pos.y, pos.z);
   }
   void DefaultVertex::setPosition(f32 x, f32 y, f32 z)
   {
      position.x = x;
      position.y = y;
      position.z = z;
      position.w = 0;
   }
   void DefaultVertex::setColor(Color color)
   {
      setColor(color.r, color.g, color.b, color.a);
   }
   void DefaultVertex::setColor(f32 r, f32 g, f32 b, f32 a)
   {
      diffuse.x = r;
      diffuse.y = g;
      diffuse.z = b;
      diffuse.w = a;
   }
   void DefaultVertex::setTextureUV(v2 textureUV)
   {
      setTextureUV(textureUV.x, textureUV.y);
   }
   void DefaultVertex::setTextureUV(f32 u, f32 v)
   {
      textureUV.x = u;
      textureUV.y = v;
   }

   InputLayout DefaultVertex::getDescription(StackAllocator& a)
   {
      u32 count = 3;
      CORE_ASSERT_ERR((a.size - a.allocated) >= sizeof(D3D11_INPUT_ELEMENT_DESC) * count, "Not enough memory to generate DefaultVertex layout information");
      InputLayout layout{allocate<D3D11_INPUT_ELEMENT_DESC>(a, count), 0};
      layout.buffer[layout.size++] = {"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0};
      layout.buffer[layout.size++] = {"DIFFUSE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0};
      layout.buffer[layout.size++] = {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0};
      return layout;
   }

   //////////// HEALTH VERTEX

   HealthVertex::HealthVertex()
      : HealthVertex({}, {}, {-1, -1}, 0)
   {
   }
   HealthVertex::HealthVertex(v3 position, Color color, v2 textureUV, f32 health)
      : DefaultVertex(position, color, textureUV), health(health, 0)
   {
   }

   void HealthVertex::setHealth(f32 h)
   {
      health.x = h;
      health.y = 0;
   }

   InputLayout HealthVertex::getDescription(StackAllocator& a)
   {
      u32 count = 4;
      CORE_ASSERT_ERR((a.size - a.allocated) >= sizeof(D3D11_INPUT_ELEMENT_DESC) * count, "Not enough memory to generate HealthVertex layout information");
      InputLayout layout{allocate<D3D11_INPUT_ELEMENT_DESC>(a, count), 0};
      layout.buffer[layout.size++] = {"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0};
      layout.buffer[layout.size++] = {"DIFFUSE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0};
      layout.buffer[layout.size++] = {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0};
      layout.buffer[layout.size++] = {"HEALTH", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0};
      return layout;

      /*{
         {"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
         {"DIFFUSE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
         {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
         {"HEALTH", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
         
         //{"WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},
         //{"WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
         //{"WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
         //{"WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
         //{"FILL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
         
      };*/
   }
}