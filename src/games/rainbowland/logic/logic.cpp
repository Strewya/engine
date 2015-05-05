//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "games/rainbowland/logic/logic.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "games/rainbowland/types/types.h"
#include "input/keyboard.h"
#include "util/utility.h"
#include "window/window_event.h"
/******* end headers *******/

namespace Core
{
   void simulateMovement(float dt, VectorOfMovables& movables)
   {
      for( auto& mover : movables )
      {
         auto acceleration = mover.direction*mover.acceleration;
         acceleration += -mover.velocity*10.0f;
         mover.position = acceleration*0.5f*dt*dt + mover.velocity*dt + mover.position;
         mover.velocity = acceleration*dt + mover.velocity;
      }
   }

   bool equalEvents(const WindowEvent& l, const WindowEvent& r)
   {
      if( l.type != r.type ) return false;
      switch( l.type )
      {
         case WE_KEYBOARDKEY:
         {
            if( l.keyboard.firstTimeDown != r.keyboard.firstTimeDown ) return false;
            if( l.keyboard.key.isDown != r.keyboard.key.isDown ) return false;
            if( l.keyboard.key.id != r.keyboard.key.id ) return false;
         } break;
         case WE_GAMEPADAXIS:
         {
            if( l.gamepad.id != r.gamepad.id ) return false;
            if( l.gamepad.axis.id != r.gamepad.axis.id ) return false;
         } break;
         case WE_GAMEPADBUTTON:
         {
            if( l.gamepad.id != r.gamepad.id ) return false;
            if( l.gamepad.button.id != l.gamepad.button.id ) return false;
            if( l.gamepad.button.isDown != r.gamepad.button.isDown ) return false;
         } break;
         case WE_GAMEPADCONNECTION:
         {
            if( l.gamepad.id != r.gamepad.id ) return false;
            if( l.gamepad.connection.status != r.gamepad.connection.status ) return false;
         } break;
         default:
         {
            return false;
         } break;
      }
      return true;
   }

   VectorOfGameEvents translateWindowEventsToGameEvents(VectorOfWindowEvents& events, VectorOfTranslationData& translationData)
   {
      VectorOfGameEvents gameEvents;
      for( auto& e : events )
      {
         for( auto& td : translationData )
         {
            if( equalEvents(e, td.windowEvent) )
            {
               gameEvents.push_back(td.gameEvent);
            }
         }
      }
      return gameEvents;
   }

   void modifyPlayerDirectionTargets(VectorOfGameEvents& events, VectorOfMoveDirectionTargets& directions)
   {
      VectorOfGameEvents newEvents;
      for( auto e : events )
      {
         switch( e.type )
         {
            case GE_DIRECTION_CHANGE:
            {
               auto& target = directions[e.directionChange.playerId].direction;
               auto change = e.directionChange.direction;
               target = target + change;
            } break;
         }
      }
      events.insert(events.end(), newEvents.begin(), newEvents.end());
   }

   void updatePlayerMovementDirection(float dt, VectorOfMoveDirectionTargets& directions, VectorOfMovables& movables)
   {
      for( auto& dir : directions )
      {
         auto& mover = movables[dir.objId];
         mover.acceleration = 60.0f*(vec2f::length2(dir.direction) > 0.0f ? 1 : 0);
         if( mover.acceleration > 0.0f )
         {
            float currentRad = std::atan2f(mover.direction.y, mover.direction.x);
            float targetRad = std::atan2f(dir.direction.y, dir.direction.x);
            float currentDeg = Rad2Deg(currentRad);
            float targetDeg = Rad2Deg(targetRad);

            auto wra = targetDeg - currentDeg;
            if( wra > 180.0f ) wra -= 360.0f;
            if( wra < -180.0f ) wra += 360.0f;

            currentDeg += wra * 4 * dt; // player.objectTimer.getDeltaSeconds();
            currentRad = Deg2Rad(currentDeg);

            mover.direction.x = std::cosf(currentRad);
            mover.direction.y = std::sinf(currentRad);
         }
      }
   }

   void updateOrientation(VectorOfAimDirections& aimDirections)
   {

   }

   void updateAim(VectorOfAimDirections& aimDirections)
   {

   }
}