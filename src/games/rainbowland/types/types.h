#pragma once
/********************************************
*  contents:   #description
*  usage:      
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include "games/util/entity.h"
#include "util/geometry/vec_types.h"
#include "window/window_event.h"
/******* end header inclusion *******/

namespace core
{
   struct GameEvent_DirectionChange
   {
      uint32_t playerId;
      Vec2 direction;
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
      Vec2 direction;
   };

   struct AimDirection
   {
      uint32_t objId;
      Vec2 direction;
   };


   struct MovingThing
   {
      float acceleration;
      Vec2 velocity;
      Vec2 direction;
      Vec2 position;
   };
}