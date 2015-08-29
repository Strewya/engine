#pragma once
/********************************************
*  contents:   #description
*  usage:
********************************************/
/******* c++ headers *******/
#include <unordered_map>
#include <vector>
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   struct Entity;
   
   struct MovementData
   {
   public:
      MovementData();

      void allocate(u32 newSize);
      void make(Entity e);
      void destroy(Entity e);

      u32 getEntitySlot(Entity e);

      Entity* _entity;
      f32* _acceleration;
      v2* _velocity;
      v2* _direction;
      v2* _position;

      u32 _inUse;

   private:
      u32 m_allocated;
      std::vector<u8> m_memory;
      std::unordered_map<u32, u32> m_map;

      u32 getDataIndex(u32 c);
      u32 makeComponent(u32 idx);
   };

   void doMovement(f32 dt, MovementData& data);
}
