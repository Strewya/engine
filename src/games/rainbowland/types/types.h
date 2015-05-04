#pragma once
/********************************************
*  contents:   #todo
*  usage:      
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include "util/geometry/vec2.h"
#include "window/window_event.h"
/******* end header inclusion *******/

namespace Core
{
   struct GameEvent_DirectionChange
   {
      uint32_t playerId;
      Vec2f direction;
   };

   struct GameEvent_AccelerationChange
   {
      uint32_t playerId;
      float acceleration;
   };

   enum GameEventType
   {
      GE_DIRECTION_CHANGE,
      GE_ACCELERATION_CHANGE
   };

   class GameEvent
   {
   public:
      GameEventType type;
      union
      {
         GameEvent_AccelerationChange accelerationChange;
         GameEvent_DirectionChange directionChange;
      };
   };


   struct Window2GameEvent
   {
      WindowEvent windowEvent;
      GameEvent gameEvent;
   };


   class DirectionTarget
   {
   public:
      uint32_t objId;
      Vec2f direction;
   };


   class MovingThing
   {
   public:
      float acceleration;
      Vec2f velocity;
      Vec2f direction;
      Vec2f position;
   };
}