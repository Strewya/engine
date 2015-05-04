#pragma once
/********************************************
*  contents:   #todo
*  usage:      
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include "util/geometry/vec2.h"
/******* end header inclusion *******/

namespace Core
{
   struct GameEvent_DirectionChange
   {
      uint32_t playerId;
      Vec2f direction;
   };

   enum GameEventType
   {
      GE_DIRECTION_CHANGE
   };

   class GameEvent
   {
   public:
      GameEventType type;
      union
      {
         GameEvent_DirectionChange directionChange;
      };
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