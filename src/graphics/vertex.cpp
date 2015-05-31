//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "graphics/vertex.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "util/color.h"
#include "util/geometry/vec_types.h"
/******* end headers *******/

namespace core
{
   //////////////// DEFAULT VERTEX

   DefaultVertex::DefaultVertex()
      : DefaultVertex({}, {}, {-1, -1})
   {
   }

   DefaultVertex::DefaultVertex(Vec3 position, Color color, Vec2 textureUV)
      : position(position.x, position.y, position.z, 0),
      diffuse(color.r, color.g, color.b, color.a),
      textureUV(textureUV.x, textureUV.y)
   {
   }

   void DefaultVertex::setPosition(Vec3 pos)
   {
      setPosition(pos.x, pos.y, pos.z);
   }
   void DefaultVertex::setPosition(float x, float y, float z)
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
   void DefaultVertex::setColor(float r, float g, float b, float a)
   {
      diffuse.x = r;
      diffuse.y = g;
      diffuse.z = b;
      diffuse.w = a;
   }
   void DefaultVertex::setTextureUV(Vec2 textureUV)
   {
      setTextureUV(textureUV.x, textureUV.y);
   }
   void DefaultVertex::setTextureUV(float u, float v)
   {
      textureUV.x = u;
      textureUV.y = v;
   }

   InputLayout DefaultVertex::getDescription()
   {
      return
      {
         {"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
         {"DIFFUSE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
         {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
      };
   }

   //////////// HEALTH VERTEX

   HealthVertex::HealthVertex()
      : HealthVertex({}, {}, {-1, -1}, 0)
   {
   }
   HealthVertex::HealthVertex(Vec3 position, Color color, Vec2 textureUV, float health)
      : DefaultVertex(position, color, textureUV), health(health, 0)
   {
   }

   void HealthVertex::setHealth(float h)
   {
      health.x = h;
      health.y = 0;
   }

   InputLayout HealthVertex::getDescription()
   {
      auto parentInputLayout = DefaultVertex::getDescription();
      parentInputLayout.push_back({"HEALTH", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0});
      return parentInputLayout;
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