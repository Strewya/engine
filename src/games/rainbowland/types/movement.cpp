//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "games/rainbowland/types/movement.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "games/util/entity.h"
#include "utility/geometry/vec2.h"
/******* end headers *******/

namespace core
{
   MovementData::MovementData()
      : _entity(nullptr), _acceleration(nullptr), _velocity(nullptr),
      _direction(nullptr), _position(nullptr), _inUse(0), m_allocated(0)
   {}

   void MovementData::allocate(uint32_t newSize)
   {
      if( newSize <= m_allocated )
      {
         return;
      }
      std::vector<uint8_t> newMemory;
      auto newByteCount = newSize*(sizeof(Entity) + sizeof(float) + sizeof(Vec2) * 3);
      newMemory.resize(newByteCount);

      Entity* newEntity = (Entity*)newMemory.data();
      float* newAcceleration = (float*)(newEntity + newSize);
      Vec2* newVelocity = (Vec2*)(newAcceleration + newSize);
      Vec2* newDirection = (Vec2*)(newVelocity + newSize);
      Vec2* newPosition = (Vec2*)(newDirection + newSize);

      memcpy(newEntity, _entity, _inUse*sizeof(Entity));
      memcpy(newAcceleration, _acceleration, _inUse*sizeof(float));
      memcpy(newVelocity, _velocity, _inUse*sizeof(Vec2));
      memcpy(newDirection, _direction, _inUse*sizeof(Vec2));
      memcpy(newPosition, _position, _inUse*sizeof(Vec2));

      std::swap(m_memory, newMemory);
      _entity = newEntity;
      _acceleration = newAcceleration;
      _velocity = newVelocity;
      _direction = newDirection;
      _position = newPosition;

      m_allocated = newSize;
   }

   void MovementData::make(Entity e)
   {
      if( _inUse >= m_allocated )
      {
         allocate(m_allocated * 2);
      }
      m_map.emplace(e.id, _inUse);
      ++_inUse;
   }

   void MovementData::destroy(Entity e)
   {
      uint32_t c = makeComponent(e.id);
      auto i = getDataIndex(c);
      auto last = _inUse - 1;
      Entity last_e = _entity[last];

      _entity[i] = _entity[last];
      _acceleration[i] = _acceleration[last];
      _velocity[i] = _velocity[last];
      _direction[i] = _direction[last];
      _position[i] = _position[last];

      m_map[last_e.id] = i;
      m_map.erase(i);

      --_inUse;
   }

   uint32_t MovementData::getEntitySlot(Entity e)
   {
      uint32_t result = 0;
      auto it = m_map.find(e.id);
      if( it != m_map.end() )
      {
         result = makeComponent(it->second);
      }

//       result.entity = _entity + index;
//       result.acceleration = _acceleration + index;
//       result.velocity = _velocity + index;
//       result.direction = _direction + index;
//       result.position = _position + index;

      return result;
   }

   void doMovement(float dt, MovementData& data)
   {
      for( uint32_t i = 0; i < data._inUse; ++i )
      {
         auto acceleration = data._direction[i] * data._acceleration[i];
         acceleration += -data._velocity[i] * 10.0f;
         data._position[i] = acceleration*0.5f*dt*dt + data._velocity[i] * dt + data._position[i];
         data._velocity[i] = acceleration*dt + data._velocity[i];
      }
   }

   uint32_t MovementData::getDataIndex(uint32_t c)
   {
      auto result = c - 1;
      return result;
   }
   uint32_t MovementData::makeComponent(uint32_t idx)
   {
      uint32_t result{idx + 1};
      return result;
   }
}