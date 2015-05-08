#pragma once
/********************************************
*  contents:   #description
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
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

   
}