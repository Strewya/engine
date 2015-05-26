#pragma once
/********************************************
*  contents:   vertex types declarations
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "graphics/dx_include.h"
/******* extra headers *******/
#include "graphics/graphics_types.h"
#include "util/geometry/vec2_fwd.h"
#include "util/geometry/vec3_fwd.h"
/******* end header inclusion *******/

namespace core
{
   struct Color;

   struct DefaultVertex
   {
      DefaultVertex();
      DefaultVertex(vec3f position, Color color, vec2f textureUV);
      void setPosition(vec3f position);
      void setPosition(float x, float y, float z);
      void setColor(Color color);
      void setColor(float r, float g, float b, float a);
      void setTextureUV(vec2f textureUV);
      void setTextureUV(float u, float v);

      XMFLOAT4 position;
      XMFLOAT4 diffuse;
      XMFLOAT2 textureUV;

      static InputLayout getDescription();
   };

   struct HealthVertex : public DefaultVertex
   {
      HealthVertex();
      HealthVertex(vec3f position, Color color, vec2f textureUV, float health);
      void setHealth(float h);

      XMFLOAT2 health;

      static InputLayout getDescription();
   };
}