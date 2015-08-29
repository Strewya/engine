//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "games/rainbowland/types/movement.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "games/entity.h"
#include "utility/geometry/vec2.h"
/******* end headers *******/

namespace core
{
   MovementData::MovementData()
      : _entity(nullptr), _acceleration(nullptr), _velocity(nullptr),
      _direction(nullptr), _position(nullptr), _inUse(0), m_allocated(0)
   {}

   void MovementData::allocate(u32 newSize)
   {
      if( newSize <= m_allocated )
      {
         return;
      }
      std::vector<u8> newMemory;
      auto newByteCount = newSize*(sizeof(Entity) + sizeof(f32) + sizeof(v2) * 3);
      newMemory.resize(newByteCount);

      Entity* newEntity = (Entity*)newMemory.data();
      f32* newAcceleration = (f32*)(newEntity + newSize);
      v2* newVelocity = (v2*)(newAcceleration + newSize);
      v2* newDirection = (v2*)(newVelocity + newSize);
      v2* newPosition = (v2*)(newDirection + newSize);

      memcpy(newEntity, _entity, _inUse*sizeof(Entity));
      memcpy(newAcceleration, _acceleration, _inUse*sizeof(f32));
      memcpy(newVelocity, _velocity, _inUse*sizeof(v2));
      memcpy(newDirection, _direction, _inUse*sizeof(v2));
      memcpy(newPosition, _position, _inUse*sizeof(v2));

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
      u32 c = makeComponent(e.id);
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

   u32 MovementData::getEntitySlot(Entity e)
   {
      u32 result = 0;
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

   void doMovement(f32 dt, MovementData& data)
   {
      for( u32 i = 0; i < data._inUse; ++i )
      {
         auto acceleration = data._direction[i] * data._acceleration[i];
         acceleration += -data._velocity[i] * 10.0f;
         data._position[i] = acceleration*0.5f*dt*dt + data._velocity[i] * dt + data._position[i];
         data._velocity[i] = acceleration*dt + data._velocity[i];
      }
   }

   u32 MovementData::getDataIndex(u32 c)
   {
      auto result = c - 1;
      return result;
   }
   u32 MovementData::makeComponent(u32 idx)
   {
      u32 result{idx + 1};
      return result;
   }
}