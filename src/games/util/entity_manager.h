#pragma once
/********************************************
*  contents:   #description
*  usage:
********************************************/
/******* c++ headers *******/
#include <cstdint>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include "util/ring_buffer.h"
/******* end header inclusion *******/

namespace core
{
   struct EntityHandle;

   struct EntityManager
   {
   public:
      EntityHandle create();
      bool alive(EntityHandle e);
      void kill(EntityHandle e);
      
   private:
      RingBuffer m_freeIndex;
      std::vector<uint8_t> m_generation;
   };
}