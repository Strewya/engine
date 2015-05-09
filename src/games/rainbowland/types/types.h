#pragma once
/********************************************
*  contents:   #description
*  usage:      
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
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

}