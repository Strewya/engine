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

   VectorOfGameEvents translateWindowEventsToGameEvents(VectorOfWindowEvents& events)
   {
      VectorOfGameEvents gameEvents;
      for( auto& e : events )
      {
         if( e.type == WE_KEYBOARDKEY )
         {
            switch( e.keyboard.key.id )
            {
               case Keyboard::S:
               {
                  if( e.keyboard.firstTimeDown )
                     gameEvents.emplace_back(GameEvent{GE_DIRECTION_CHANGE, {GameEvent_DirectionChange{0, Vec2f{0.0f, -1.0f}}}});
                  else if( !e.keyboard.key.isDown )
                     gameEvents.emplace_back(GameEvent{GE_DIRECTION_CHANGE, {GameEvent_DirectionChange{0, Vec2f{0.0f, +1.0f}}}});
               } break;
               case Keyboard::W:
               {
                  if( e.keyboard.firstTimeDown )
                     gameEvents.emplace_back(GameEvent{GE_DIRECTION_CHANGE, {GameEvent_DirectionChange{0, Vec2f{0.0f, +1.0f}}}});
                  else if( !e.keyboard.key.isDown )
                     gameEvents.emplace_back(GameEvent{GE_DIRECTION_CHANGE, {GameEvent_DirectionChange{0, Vec2f{0.0f, -1.0f}}}});
               } break;
               case Keyboard::A:
               {
                  if( e.keyboard.firstTimeDown )
                     gameEvents.emplace_back(GameEvent{GE_DIRECTION_CHANGE, {GameEvent_DirectionChange{0, Vec2f{-1.0f, 0.0f}}}});
                  else if( !e.keyboard.key.isDown )
                     gameEvents.emplace_back(GameEvent{GE_DIRECTION_CHANGE, {GameEvent_DirectionChange{0, Vec2f{+1.0f, 0.0f}}}});
               } break;
               case Keyboard::D:
               {
                  if( e.keyboard.firstTimeDown )
                     gameEvents.emplace_back(GameEvent{GE_DIRECTION_CHANGE, {GameEvent_DirectionChange{0, Vec2f{+1.0f, 0.0f}}}});
                  else if( !e.keyboard.key.isDown )
                     gameEvents.emplace_back(GameEvent{GE_DIRECTION_CHANGE, {GameEvent_DirectionChange{0, Vec2f{-1.0f, 0.0f}}}});
               } break;
            }
         }
      }
      return gameEvents;
   }

   void modifyPlayerDirectionTargets(VectorOfGameEvents& events, VectorOfDirectionTargets& directions)
   {
      for( auto e : events )
      {
         switch( e.type )
         {
            case GE_DIRECTION_CHANGE:
            {
               directions[e.directionChange.playerId].direction += e.directionChange.direction;
            } break;
         }
      }
   }

   void updatePlayerMovementDirection(float dt, VectorOfDirectionTargets& directions, VectorOfMovables& movables)
   {
      for( auto dir : directions )
      {
         auto& mover = movables[dir.objId];

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

   /*
   void moveMonsters()
   {
   for( auto* monster_ptr : game.m_monsters )
   {
   auto& monster = *monster_ptr;
   auto n = Vec2f::length(steer_arrive(monster.transform.position, monster.maxSpeed, monster.brain.targetLocation));
   monster.transform.position += monster.direction*n*monster.objectTimer.getDeltaSeconds();

   updateMonsterInGrid(game.m_monsterGrid, monster);

   separateMonsters(monster, game.m_monsterGrid);
   }
   }

   void movePlayers()
   {
   for( Player& player : game.m_players )
   {
   if( !player.isAimRelative )
   {
   if( player.directionActive[Up] || player.directionActive[Down] || player.directionActive[Left] || player.directionActive[Right] )
   {
   player.currentSpeed = player.maxSpeed;

   Vec2f targetVector;
   if( player.directionActive[Up] )
   {
   targetVector.y += 1;
   }
   if( player.directionActive[Down] )
   {
   targetVector.y -= 1;
   }
   if( player.directionActive[Left] )
   {
   targetVector.x -= 1;
   }
   if( player.directionActive[Right] )
   {
   targetVector.x += 1;
   }

   player.targetDirection = Vec2f::normalize(targetVector);
   }
   else
   {
   player.currentSpeed = 0;
   player.targetDirection = player.direction;
   }
   }

   float currentRad = std::atan2f(player.direction.y, player.direction.x);
   float targetRad = std::atan2f(player.targetDirection.y, player.targetDirection.x);
   float currentDeg = Rad2Deg(currentRad);
   float targetDeg = Rad2Deg(targetRad);

   auto wra = targetDeg - currentDeg;
   if( wra > 180.0f ) wra -= 360.0f;
   if( wra < -180.0f ) wra += 360.0f;

   currentDeg += wra * 4 * player.objectTimer.getDeltaSeconds();
   currentRad = Deg2Rad(currentDeg);

   player.direction.x = std::cosf(currentRad);
   player.direction.y = std::sinf(currentRad);

   auto velocity = player.direction*player.currentSpeed;
   player.transform.position += (player.objectTimer.getDeltaSeconds()*velocity);
   Circle playerCollider = player.collisionData;
   playerCollider.center = player.transform.position;
   if( !isCircleInsideRect(playerCollider, game.m_playingField) )
   {
   clamp(game.m_playingField.left() + player.collisionData.radius,
   game.m_playingField.right() - player.collisionData.radius,
   player.transform.position.x);
   clamp(game.m_playingField.bottom() + player.collisionData.radius,
   game.m_playingField.top() - player.collisionData.radius,
   player.transform.position.y);
   }
   }
   }
   */
}