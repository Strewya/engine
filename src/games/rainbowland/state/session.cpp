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
      state.square.set(0, 0, 0.5f, 0.5f);
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

      auto move = 0.2f;

      if( contains(Keyboard::W, STATE) )
      {
         state.square.center.y += move;
      }
      if( contains(Keyboard::S, STATE) )
      {
         state.square.center.y -= move;
      }
      if( contains(Keyboard::A, STATE) )
      {
         state.square.center.x -= move;
      }
      if( contains(Keyboard::D, STATE) )
      {
         state.square.center.x += move;
      }

      //    preamble
      //       - update all timers
      //    player input based updates (for specific player):
      //       - start/stop moving
      //       - movement direction target
      //       - aim
      //       - start/stop weapon fire
      //       - ability use
      //       - open pause menu
      //    AI updates:
      //       - decide if should be moving
      //       - decide location to move to, calculate movement direction target
      //       - update aim based on movement direction
      //    simulation updates:
      //       - change orientation based on aim direction
      //       - update movement direction based on target direction
      //       - propose movement in current direction
      //       - players, monsters and rockets work the same
      //       - bullets could be made to work the same with some changes to their data
      //       - blasts are a bit more tricky, might have to change how they work entirely
      //       - find collisions for new position
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
      Mesh m = makeSolidQuad({}, state.square.halfSize, assets.mainVS, assets.mainPS);
      systems.gfx->renderMesh({state.square.center}, {}, m);
   }
}