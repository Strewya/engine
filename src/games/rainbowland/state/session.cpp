//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "games/rainbowland/state/session.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "games/rainbowland/resources.h"
#include "games/rainbowland/systems.h"
#include "graphics/graphics_system.h"
#include "graphics/mesh/mesh.h"
#include "input/input_system.h"
#include "input/keyboard.h"
#include "util/color.h"
#include "util/transform.h"
/******* end headers *******/

namespace core
{
   //*****************************************************************
   //          FUNCTION NAME
   //*****************************************************************
   bool session_init(SessionState& state, GameSystems systems, GameResources& assets)
   {
      return true;
   }
   
   //*****************************************************************
   //          FUNCTION NAME
   //*****************************************************************
   bool session_update(float dt, uint32_t deltaMicros, SessionState& state, GameSystems systems, GameResources& assets)
   {
      const auto& frameEvents = systems.input->getEvents();
      enum
      {
         STATE,
         ACTION,
      };
      auto contains = [&frameEvents](Keyboard::Key k, uint32_t s) -> bool
      {
         for( auto e : frameEvents )
         {
            if( e.type == WE_KEYBOARDKEY && e.keyboard.key.id == k && e.keyboard.key.isDown && (s == STATE || e.keyboard.firstTimeDown) )
            {
               return true;
            }
         }
         return false;
      };


      //    preamble
      //       - update all timers
      state.timers.advanceTime(deltaMicros);
      //    player input based updates (for specific player):
      /*
      translate window events into game events based on what the allowed controls are
      allowed actions the player should be able to do:
         
         keyboard: pressing WSAD changes the target direction of the player from (0,0) by adding a 1 to the axis, releasing WSAD subtracts the 1 off the axis
         gamepad: moving the left stick away from it's deadzone generates a normalized target direction vector which is mapped directly
            whenever the target direction is not (0,0) the player is assumed to be moving
         the aim cursor is always relative to the player
         mouse: moving the mouse should move the aim cursor relative to the player, not the screen, which generates a normalized aim direction
         gamepad: moving the right stick generates a normalized aim direction
         pressing a specific gamepad button or keyboard/mouse key activates the class ability of the player
         any extra optional actions could be considered
      */

      //the translator is a system with registered mappings from WindowEvent to GameEvent
      //done in the init function when we know how many players we have and which input device they are using
      auto gameEvents = state.inputTranslator.translate(frameEvents);
      
      std::sort(gameEvents.begin(), gameEvents.end(), [](const GameEvent& l, const GameEvent& r)
      {
         return l.type < r.type;
      });

      for( auto& ge : gameEvents )
      {
         switch( ge.type )
         {
      //       - movement direction target (implies start/stop moving)
            case GE_DIRECTION_CHANGE:
            {
               state.direction[ge.entity] += ge.direction;
               state.movement[ge.entity].acceleration() = state.constants.playerAcceleration*vec2f::length(state.direction[ge.entity]);
            } break;
      //       - aim
            case GE_AIM_CHANGE:
            {
               state.aim[ge.entity] = ge.aim;
            } break;
      //       - start/stop weapon fire
            case GE_FIRE:
            {
               state.shooting[ge.entity] = ge.shooting;
            } break;
      //       - ability use
            case GE_ABILITY:
            {
               state.activateAbility[ge.entity] = true;
            } break;
            //optional cases like opening menus, pausing the game, etc
      //       - open pause menu
         }
      }
      //    AI updates:
      //       - decide if should be moving
      //       - decide location to move to, calculate movement direction target
      //       - update aim based on movement direction

      //    simulation updates:
      //       - update movement direction based on target direction
      for( auto entity : state.entities )
      {
         state.movement[entity].direction() = calculateNewMovementDirection(vec2f::normalize(state.direction[entity]), state.movement[entity].direction());
      }
      //       - propose movement in current direction
      //       - players, monsters and rockets work the same
      //       - bullets could be made to work the same with some changes to their data
      //       - blasts are a bit more tricky, might have to change how they work entirely
      state.movement.simulate(dt);
      //       - find collisions for new position
      copyNewPositions(state.entities, state.movement, state.collision);
      auto collisionPairs = state.collision.findCollisions();
      //       - [resolve collisions via displacement vector]->optional depending on performance
      //       - accept new position with displacement vector added
      //       - fix camera to new position
      //    gameplay update:
      //       - collision response
      //       - damage calculation
      //       - bullets hurt monsters
      //       - blasts hurt monsters
      //       - rockets hurt monsters (and spawn blasts)
      //       - monsters hurt players
      //       - [players hurt monsters]->if they have a perk for it
      //       - pickup collection
      //       - bonuses
      //       - weapons
      //       - abilities
      //       - each ability has unique logic for handling collisions with all other entities
      //       - level up + perk mode (players only? maybe make the monster that makes a player kill bigger, turn him into a boss or something...)
      //       - death (monsters AND players)
      //       - active bonuses
      //       - acquired perks
      //       - fire weapons
      //       - new spawns (projectiles, blasts, monsters, pickups, blood splatter)
      //       - difficulty increase
      //    visual updates:
      //       - change orientation based on aim direction
      //       - animation
      //       - transparency

      return true;
   }
   
   //*****************************************************************
   //          FUNCTION NAME
   //*****************************************************************
   void session_render(float dt, SessionState& state, GameSystems systems, GameResources& assets)
   {
      systems.gfx->setPerspectiveProjection();
   }
}