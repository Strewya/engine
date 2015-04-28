#pragma once
/********************************************
*  contents:   #todo
*  usage:
********************************************/
/******* c++ headers *******/
#include <cstdint>
#include <vector>
/******* common headers *******/
#include <graphics/dx_include.h>
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   class DefaultVertex;
   class HealthVertex;

   typedef std::vector<DefaultVertex> DefaultVertexBuffer;
   typedef std::vector<HealthVertex> HealthVertexBuffer;

   typedef std::vector<uint32_t> IndexBuffer;

   typedef std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayout;
}
