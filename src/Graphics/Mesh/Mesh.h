#pragma once
/********************************************
*  contents:   Mesh
*  usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <Graphics/Vertex.h>
#include <Util/Vec2.h>
/******* end header inclusion *******/

namespace Core
{
   class Mesh
   {
   public:
      std::vector<Vertex> vertices;
      std::vector<uint32_t> indices;
   };
   
   Mesh makeSolidQuad(Vec2f pos, Vec2f hs);

}
