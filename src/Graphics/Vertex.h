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
/******* end header inclusion *******/

namespace Core
{
   struct VertexPosition
   {
      VertexPosition(float x=0, float y=0, float z=0);
      XMFLOAT4 m_;
      void set(float x, float y, float z);
      static D3D11_INPUT_ELEMENT_DESC getElementDesc(uint32_t offset);
   };

   struct VertexDiffuse
   {
      VertexDiffuse(float r = 0, float g = 0, float b = 0, float a = 0);
      XMFLOAT4 m_;
      void set(float r, float g, float b, float a);
      static D3D11_INPUT_ELEMENT_DESC getElementDesc(uint32_t offset);
   };

   struct VertexTextureUV
   {
      VertexTextureUV(float u = 0, float v = 0);
      XMFLOAT2 m_;
      void set(float u, float v);
      static D3D11_INPUT_ELEMENT_DESC getElementDesc(uint32_t offset);
   };



   class DefaultVertex
   {
   public:
      VertexPosition position;
      VertexDiffuse diffuse;
      VertexTextureUV texCoord;

      static std::vector<D3D11_INPUT_ELEMENT_DESC> getDescription();
   };


   struct VertexHealthValue
   {
      VertexHealthValue(float h = 0);
      XMFLOAT2 m_;
      void set(float health);
      static D3D11_INPUT_ELEMENT_DESC getElementDesc(uint32_t offset);
   };

   class Vertex
   {
   public:
      VertexPosition position;
      VertexDiffuse diffuse;
      VertexTextureUV texCoord;
      VertexHealthValue health;

      static std::vector<D3D11_INPUT_ELEMENT_DESC> getDescription();
   };


}