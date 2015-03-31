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
   class Vertex
   {
   public:
      void setPosition(float x, float y, float z);
      void setDiffuse(float r, float g, float b, float a);
      void setTextureCoords(float u, float v);

      XMFLOAT3 pos;
      XMFLOAT4 diffuse;
      XMFLOAT2 texCoord;
      XMFLOAT2 floatValue;

      static std::vector<D3D11_INPUT_ELEMENT_DESC> getDescription();
   };


}