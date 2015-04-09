#pragma once
/********************************************
*  contents:   Typedefs
*  usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <Graphics/Vertex.h>
/******* end header inclusion *******/

namespace Core
{
   typedef std::vector<DefaultVertex> DefaultVertexBuffer;
   typedef std::vector<HealthVertex> HealthVertexBuffer;

   typedef std::vector<uint32_t> IndexBuffer;
}
