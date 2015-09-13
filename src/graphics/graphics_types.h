#pragma once
/********************************************
*  contents:   types used by the graphics system and renderer
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "graphics/dx_include.h"
#include "utility/types.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   struct DefaultVertex;
   struct HealthVertex;

   enum class VertexType
   {
      Default,
      Health
   };

   struct HealthVertexBuffer
   {
      HealthVertex* data;
      u32 size;
   };

   struct DefaultVertexBuffer
   {
      DefaultVertex* data;
      u32 size;
   };

   struct IndexBuffer
   {
      u32* data;
      u32 size;
   };

   struct InputLayout
   {
      D3D11_INPUT_ELEMENT_DESC* buffer;
      u32 size;
   };

   enum VertexTopology
   {
      LineList,
      LineStrip,
      TriangleList,
      TriangleStrip,
   };
}
