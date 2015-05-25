#pragma once
/********************************************
*  contents:   #description
*  usage:      
********************************************/
/******* c++ headers *******/
#include <cstdint>
/******* common headers *******/
/******* extra headers *******/
#include "util/geometry/vec2_fwd.h"
/******* end header inclusion *******/

namespace core
{
   struct Entity;

   struct Component
   {
      // 0 is invalid
      uint32_t idx;
   };

   struct MovementData
   {
   public:
      MovementData();

      void allocate(uint32_t newSize);
      void make(Entity e);
      void destroy(Entity e);

      Component getComponentData(Entity e);

      float& acceleration(Component c);
      vec2f& velocity(Component c);
      vec2f& direction(Component c);
      vec2f& position(Component c);

      void doMovement(float dt);

   private:

      uint32_t getDataIndex(Component c);
      Component makeComponent(uint32_t idx);

      Entity* m_entity;
      float* m_acceleration;
      vec2f* m_velocity;
      vec2f* m_direction;
      vec2f* m_position;
      uint32_t m_inUse;
      uint32_t m_allocated;
      std::vector<uint8_t> m_memory;
      std::unordered_map<uint32_t, uint32_t> m_map;
   };
}