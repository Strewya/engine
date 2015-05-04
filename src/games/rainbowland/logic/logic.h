#pragma once
/********************************************
*  contents:   #todo
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   class MovingThing;
   typedef std::vector<MovingThing> VectorOfMovables;

   void simulateMovement(float dt, VectorOfMovables& movables);

   class WindowEvent;
   typedef std::vector<WindowEvent> VectorOfWindowEvents;
   class GameEvent;
   typedef std::vector<GameEvent> VectorOfGameEvents;

   VectorOfGameEvents translateWindowEventsToGameEvents(VectorOfWindowEvents& events);

   class DirectionTarget;
   typedef std::vector<DirectionTarget> VectorOfDirectionTargets;

   void modifyPlayerDirectionTargets(VectorOfGameEvents& events, VectorOfDirectionTargets& directions);

   void updatePlayerMovementDirection(float dt, VectorOfDirectionTargets& directions, VectorOfMovables& movables);
}