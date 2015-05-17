#pragma once
/********************************************
*  contents:   #description
*  usage:      
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include "games/util/entity.h"
#include "util/geometry/vec2.h"
#include "window/window_event.h"
/******* end header inclusion *******/

namespace core
{
   struct GameEvent_DirectionChange
   {
      uint32_t playerId;
      vec2f direction;
   };
   
   enum GameEventType
   {
      GE_DIRECTION_CHANGE,
      GE_AIM_CHANGE
   };

   struct GameEvent
   {
      GameEventType type;
      union
      {
         GameEvent_DirectionChange directionChange;
      };
   };


   struct Window2GameEvent
   {
      WindowEvent windowEvent;
      GameEvent gameEvent;
   };


   struct MoveDirectionTarget
   {
      uint32_t objId;
      vec2f direction;
   };

   struct AimDirection
   {
      uint32_t objId;
      vec2f direction;
   };


   struct MovingThing
   {
      float acceleration;
      vec2f velocity;
      vec2f direction;
      vec2f position;
   };



   struct Component
   {
      // 0 is invalid
      uint32_t idx;
   };

   struct MovementData
   {
   public:
      MovementData()
         : m_entity(nullptr), m_acceleration(nullptr), m_velocity(nullptr), m_direction(nullptr), m_position(nullptr), m_inUse(0), m_allocated(0)
      {}

      void allocate(uint32_t newSize)
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
      void make(Entity e)
      {
         if( m_inUse >= m_allocated )
         {
            allocate(m_allocated * 2);
         }
         m_map.emplace(e.id, m_inUse);
         ++m_inUse;
      }
      void destroy(Entity e)
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

      Component getComponentData(Entity e)
      {
         Component result{0};
         auto it = m_map.find(e.id);
         if( it != m_map.end() )
         {
            result = makeComponent(it->second);
         }
         return result;
      }

      float& acceleration(Component c)
      {
         return m_acceleration[getDataIndex(c)];
      }

      vec2f& velocity(Component c)
      {
         return m_velocity[getDataIndex(c)];
      }

      vec2f& direction(Component c)
      {
         return m_direction[getDataIndex(c)];
      }

      vec2f& position(Component c)
      {
         return m_position[getDataIndex(c)];
      }

      void doMovement(float dt)
      {
         for( uint32_t i = 0; i < m_inUse; ++i )
         {
            auto acceleration = m_direction[i]*m_acceleration[i];
            acceleration += -m_velocity[i]*10.0f;
            m_position[i] = acceleration*0.5f*dt*dt + m_velocity[i]*dt + m_position[i];
            m_velocity[i] = acceleration*dt + m_velocity[i];
         }
      }

   private:

      uint32_t getDataIndex(Component c)
      {
         auto result = c.idx - 1;
         return result;
      }
      Component makeComponent(uint32_t idx)
      {
         Component result{idx + 1};
         return result;
      }

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