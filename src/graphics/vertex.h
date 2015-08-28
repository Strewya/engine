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
#include "utility/types.h"
/******* end header inclusion *******/

namespace core
{
   struct Color;
   struct Vec2;
   struct Vec3;

   struct DefaultVertex
   {
      DefaultVertex();
      DefaultVertex(Vec3 position, Color color, Vec2 textureUV);
      void setPosition(Vec3 position);
      void setPosition(float x, float y, float z);
      void setColor(Color color);
      void setColor(float r, float g, float b, float a);
      void setTextureUV(Vec2 textureUV);
      void setTextureUV(float u, float v);

      XMFLOAT4 position;
      XMFLOAT4 diffuse;
      XMFLOAT2 textureUV;

      core_class_scope InputLayout getDescription();
   };

   struct HealthVertex : public DefaultVertex
   {
      HealthVertex();
      HealthVertex(Vec3 position, Color color, Vec2 textureUV, float health);
      void setHealth(float h);

      XMFLOAT2 health;

      core_class_scope InputLayout getDescription();
   };
}