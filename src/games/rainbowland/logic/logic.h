#pragma once
/********************************************
*  contents:   #description
*  usage:
********************************************/
/******* c++ headers *******/
#include <unordered_map>
/******* common headers *******/
/******* extra headers *******/
#include "games/util/entity_handle.h"
#include "util/geometry/vec2.h"
/******* end header inclusion *******/

namespace core
{
   struct MovingThing;
   typedef std::vector<MovingThing> VectorOfMovables;

   void simulateMovement(float dt, VectorOfMovables& movables);

   struct WindowEvent;
   typedef std::vector<WindowEvent> VectorOfWindowEvents;
   struct GameEvent;
   typedef std::vector<GameEvent> VectorOfGameEvents;
   struct Window2GameEvent;
   typedef std::vector<Window2GameEvent> VectorOfTranslationData;

   VectorOfGameEvents translateWindowEventsToGameEvents(VectorOfWindowEvents& events, VectorOfTranslationData& translationData);

   struct MoveDirectionTarget;
   typedef std::vector<MoveDirectionTarget> VectorOfMoveDirectionTargets;

   void modifyPlayerDirectionTargets(VectorOfGameEvents& events, VectorOfMoveDirectionTargets& directions);

   void updatePlayerMovementDirection(float dt, VectorOfMoveDirectionTargets& directions, VectorOfMovables& movables);

   struct AimDirection;
   typedef std::vector<AimDirection> VectorOfAimDirections;

   struct Component
   {
      // 0 is invalid
      uint32_t idx;
   };

   struct MovementData
   {
   public:
      
      void allocate(EntityHandle e);
      void deallocate(EntityHandle e);

      Component getComponentData(EntityHandle e);

      float& acceleration(Component c)
      {
         return m_acceleration[c.idx];
      }

   private:

      uint32_t fetchIndex(Component c);
      Component storeIndex(uint32_t idx);

      float* m_acceleration;
      vec2f* m_velocity;
      vec2f* m_direction;
      vec2f* m_position;
      std::vector<uint8_t> m_memory;
      std::unordered_map<uint32_t, uint32_t> m_map;
   };

}
