#pragma once
/********************************************
*  class:   Vertex
*  usage:
********************************************/
/******* C++ headers *******/
#include <vector>
/******* common headers *******/
#include <Graphics/DXInclude.h>
/******* extra headers *******/
#include <Util/Vec2Fwd.h>
#include <Util/Vec3Fwd.h>
/******* end header inclusion *******/

namespace Core
{
   class Color;

   class DefaultVertex
   {
   public:
      DefaultVertex();
      DefaultVertex(Vec3f position, Color color, Vec2f textureUV);
      void setPosition(Vec3f position);
      void setPosition(float x, float y, float z);
      void setColor(Color color);
      void setColor(float r, float g, float b, float a);
      void setTextureUV(Vec2f textureUV);
      void setTextureUV(float u, float v);

      XMFLOAT4 position;
      XMFLOAT4 diffuse;
      XMFLOAT2 textureUV;

      static std::vector<D3D11_INPUT_ELEMENT_DESC> getDescription();
   };

   class HealthVertex : public DefaultVertex
   {
   public:
      HealthVertex();
      HealthVertex(Vec3f position, Color color, Vec2f textureUV, float health);
      void setHealth(float h);

      XMFLOAT2 health;

      static std::vector<D3D11_INPUT_ELEMENT_DESC> getDescription();
   };
}