#pragma once
/********************************************
*  contents:   vertex types declarations
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "graphics/dx_include.h"
#include "utility/types.h"
/******* extra headers *******/
#include "graphics/graphics_types.h"
/******* end header inclusion *******/

namespace core
{
   struct Color;
   struct StackAllocator;
   
   struct DefaultVertex
   {
      DefaultVertex();
      DefaultVertex(v3 position, Color color, v2 textureUV);
      void setPosition(v3 position);
      void setPosition(f32 x, f32 y, f32 z);
      void setColor(Color color);
      void setColor(f32 r, f32 g, f32 b, f32 a);
      void setTextureUV(v2 textureUV);
      void setTextureUV(f32 u, f32 v);

      XMFLOAT4 position;
      XMFLOAT4 diffuse;
      XMFLOAT2 textureUV;

      core_class_scope InputLayout getDescription();
   };

   struct HealthVertex : public DefaultVertex
   {
      HealthVertex();
      HealthVertex(v3 position, Color color, v2 textureUV, f32 health);
      void setHealth(f32 h);

      XMFLOAT2 health;

      core_class_scope InputLayout getDescription();
   };
}