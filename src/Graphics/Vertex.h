#pragma once
/********************************************
*  contents:   #todo
*  usage:
********************************************/
/******* c++ headers *******/
#include <vector>
/******* common headers *******/
#include "graphics/dx_include.h"
/******* extra headers *******/
#include "graphics/graphics_typedefs.h"
#include "util/vec2_fwd.h"
#include "util/vec3_fwd.h"
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

      static InputLayout getDescription();
   };

   class HealthVertex : public DefaultVertex
   {
   public:
      HealthVertex();
      HealthVertex(Vec3f position, Color color, Vec2f textureUV, float health);
      void setHealth(float h);

      XMFLOAT2 health;

      static InputLayout getDescription();
   };
}