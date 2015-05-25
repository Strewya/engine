//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "games/rainbowland/types/movement.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "games/util/entity.h"
#include "util/geometry/vec2.h"
/******* end headers *******/

namespace core
{
   MovementData::MovementData()
      : m_entity(nullptr), m_acceleration(nullptr), m_velocity(nullptr),
      m_direction(nullptr), m_position(nullptr), m_inUse(0), m_allocated(0)
   {}

   void MovementData::allocate(uint32_t newSize)
   {
      if( newSize <= m_allocated )
      {
         return;
      }
      std::vector<uint8_t> newMemory;
      auto newByteCount = newSize*(sizeof(Entity) + sizeof(float) + sizeof(vec2f) * 3);
      newMemory.resize(newByteCount);

      Entity* newEntity = (Entity*)newMemory.data();
      float* newAcceleration = (float*)(newEntity + newSize);
      vec2f* newVelocity = (vec2f*)(newAcceleration + newSize);
      vec2f* newDirection = (vec2f*)(newVelocity + newSize);
      vec2f* newPosition = (vec2f*)(newDirection + newSize);

      memcpy(newEntity, m_entity, m_inUse*sizeof(Entity));
      memcpy(newAcceleration, m_acceleration, m_inUse*sizeof(float));
      memcpy(newVelocity, m_velocity, m_inUse*sizeof(vec2f));
      memcpy(newDirection, m_direction, m_inUse*sizeof(vec2f));
      memcpy(newPosition, m_position, m_inUse*sizeof(vec2f));

      std::swap(m_memory, newMemory);
      m_entity = newEntity;
      m_acceleration = newAcceleration;
      m_velocity = newVelocity;
      m_direction = newDirection;
      m_position = newPosition;

      m_allocated = newSize;
   }

   void MovementData::make(Entity e)
   {
      if( m_inUse >= m_allocated )
      {
         allocate(m_allocated * 2);
      }
      m_map.emplace(e.id, m_inUse);
      ++m_inUse;
   }

   void MovementData::destroy(Entity e)
   {
      Component c = getComponentData(e);
      auto i = getDataIndex(c);
      auto last = m_inUse - 1;
      Entity last_e = m_entity[last];

      m_entity[i] = m_entity[last];
      m_acceleration[i] = m_acceleration[last];
      m_velocity[i] = m_velocity[last];
      m_direction[i] = m_direction[last];
      m_position[i] = m_position[last];

      m_map[last_e.id] = i;
      m_map.erase(i);

      --m_inUse;
   }

   Component MovementData::getComponentData(Entity e)
   {
      Component result{0};
      auto it = m_map.find(e.id);
      if( it != m_map.end() )
      {
         result = makeComponent(it->second);
      }
      return result;
   }

   float& MovementData::acceleration(Component c)
   {
      return m_acceleration[getDataIndex(c)];
   }

   vec2f& MovementData::velocity(Component c)
   {
      return m_velocity[getDataIndex(c)];
   }

   vec2f& MovementData::direction(Component c)
   {
      return m_direction[getDataIndex(c)];
   }

   vec2f& MovementData::position(Component c)
   {
      return m_position[getDataIndex(c)];
   }

   void MovementData::doMovement(float dt)
   {
      for( uint32_t i = 0; i < m_inUse; ++i )
      {
         auto acceleration = m_direction[i] * m_acceleration[i];
         acceleration += -m_velocity[i] * 10.0f;
         m_position[i] = acceleration*0.5f*dt*dt + m_velocity[i] * dt + m_position[i];
         m_velocity[i] = acceleration*dt + m_velocity[i];
      }
   }

   uint32_t MovementData::getDataIndex(Component c)
   {
      auto result = c.idx - 1;
      return result;
   }
   Component MovementData::makeComponent(uint32_t idx)
   {
      Component result{idx + 1};
      return result;
   }
}