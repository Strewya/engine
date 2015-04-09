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
   class DefaultVertex
   {
   public:
      XMFLOAT4 position;
      XMFLOAT4 diffuse;
      XMFLOAT2 textureUV;

      static std::vector<D3D11_INPUT_ELEMENT_DESC> getDescription();
   };

   class HealthVertex
   {
   public:
      XMFLOAT4 position;
      XMFLOAT4 diffuse;
      XMFLOAT2 textureUV;
      XMFLOAT2 health;

      static std::vector<D3D11_INPUT_ELEMENT_DESC> getDescription();
   };
}