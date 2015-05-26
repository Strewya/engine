//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "games/rainbowland/main_state.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "games/rainbowland/resources.h"
#include "games/rainbowland/systems.h"
#include "graphics/graphics_system.h"
#include "input/input_system.h"
#include "input/keyboard.h"
#include "util/utility.h"
#include "util/color.h"
#include "util/transform.h"

#include "graphics/font/font_system.h"
#include "graphics/mesh/mesh.h"
/******* end headers *******/

namespace core
{
   bool initializeGameState(Timer& timer, GameState& state, GameSystems& systems, GameResources& assets)
   {
      state.globalGameState = GameState::GlobalGameState::MainMenu;
      state.gameplayState = GameState::GameplayState::ClassPick;

      state.fontDesc = loadFont(CORE_RESOURCE("Defs/font.font"), assets.font, assets.mainVS, assets.mainPS);

      return true;
   }

   vec2i tj{Left, Top};

   bool updateGameState(float dt_s, uint32_t dt_us, GameState& state, GameSystems& systems, GameResources& assets)
   {
      bool stillRunning = true;

      /*
      attempt at a complete phasing of update loop

      preamble
      - update all timers
      player input based updates (for specific player):
      - start/stop moving
      - movement direction target
      - aim
      - start/stop weapon fire
      - ability use
      - open pause menu
      AI updates:
      - decide if should be moving
      - decide location to move to, calculate movement direction target
      - update aim based on movement direction
      simulation updates:
      - change orientation based on aim direction
      - update movement direction based on target direction
      - propose movement in current direction
      - players, monsters and rockets work the same
      - bullets could be made to work the same with some changes to their data
      - blasts are a bit more tricky, might have to change how they work entirely
      - find collisions for new position
      - [resolve collisions via displacement vector]->optional depending on performance
      - accept new position with displacement vector added
      - fix camera to new position
      gameplay update:
      - collision response
      - damage calculation
      - bullets hurt monsters
      - blasts hurt monsters
      - rockets hurt monsters (and spawn blasts)
      - monsters hurt players
      - [players hurt monsters]->if they have a perk for it
      - pickup collection
      - bonuses
      - weapons
      - abilities
      - each ability has unique logic for handling collisions with all other entities
      - level up + perk mode (players only? maybe make the monster that makes a player kill bigger, turn him into a boss or something...)
      - death (monsters AND players)
      - active bonuses
      - acquired perks
      - fire weapons
      - new spawns (projectiles, blasts, monsters, pickups, blood splatter)
      - difficulty increase
      visual updates:
      - animation
      - transparency
      -

      */
      auto eventList = systems.input->getEvents();
      enum
      {
         STATE,
         ACTION,
      };
      auto contains = [&eventList](Keyboard::Key k, uint32_t s) -> bool
      {
         for( auto e : eventList )
         {
            if( e.type == WE_KEYBOARDKEY && e.keyboard.key.id == k && (s == STATE || e.keyboard.firstTimeDown) )
            {
               return true;
            }
         }
         return false;
      };


      

      switch( state.globalGameState )
      {
         case GameState::GlobalGameState::MainMenu:
         {
            if( contains(Keyboard::Space, ACTION) )
            {
               state.globalGameState = GameState::GlobalGameState::Gameplay;
               state.gameplayState = GameState::GameplayState::ClassPick;
            }
         } break;
         case GameState::GlobalGameState::Gameplay:
         {
            switch( state.gameplayState )
            {
               case GameState::GameplayState::ClassPick:
               {
                  if( contains(Keyboard::Space, ACTION) )
                  {
                     state.gameplayState = GameState::GameplayState::Session;
                  }
               } break;
               case GameState::GameplayState::Session:
               {
                  if( contains(Keyboard::Space, ACTION) )
                  {
                     state.globalGameState = GameState::GlobalGameState::Score;
                  }
               } break;
            }
         } break;
         case GameState::GlobalGameState::Score:
         {
            if( contains(Keyboard::Space, ACTION) )
            {
               state.globalGameState = GameState::GlobalGameState::MainMenu;
            }
         } break;
      }

      return stillRunning;
   }

   void renderGameState(float dt, GameState& state, GameSystems& systems, GameResources& assets)
   {
      systems.gfx->setPerspectiveProjection();
/*
      systems.gfx->setTransparency(false);
      systems.gfx->renderMesh(Transform{}, Color{}, state.backgroundMesh);
      systems.gfx->setTransparency(true);
      systems.gfx->renderMesh(Transform{state.movingThings[0].position}, Color{}, state.playerMesh);
      systems.gfx->renderMesh(Transform{state.movingThings[0].position + state.movingThings[0].direction, {0.1f, 0.1f}}, Color{}, makeSolidQuad({}, {1, 1}, assets.mainVS, assets.mainPS));
*/

      FontSystem fontSystem;
      fontSystem.init(systems.gfx->textures);

      systems.gfx->setOrthographicProjection();
      systems.gfx->setTransparency(true);

      

      switch( state.globalGameState )
      {
         case GameState::GlobalGameState::MainMenu:
         {
            auto mesh = fontSystem.makeTextMesh("Main menu", state.fontDesc, {1, 1}, {Left, Middle});
            systems.gfx->renderMesh({}, {}, mesh);
         } break;
         case GameState::GlobalGameState::Gameplay:
         {
            auto mesh = fontSystem.makeTextMesh("Gameplay", state.fontDesc, {1, 1}, {Center, Middle}, Rect{{}, {200, 20}});
            systems.gfx->renderMesh({}, {}, mesh);
            switch( state.gameplayState )
            {
               case GameState::GameplayState::ClassPick:
               {
                  mesh = fontSystem.makeTextMesh("Class picking", state.fontDesc, {1, 1}, {Center, Middle}, Rect{{}, {200, 20}});
                  systems.gfx->renderMesh({vec2f{0, -40}}, {}, mesh);
               } break;
               case GameState::GameplayState::Session:
               {
                  mesh = fontSystem.makeTextMesh("Session", state.fontDesc, {1, 1}, {Center, Middle}, Rect{{}, {200, 20}});
                  systems.gfx->renderMesh({vec2f{0, -40}}, {}, mesh);
               } break;
            }
         } break;
         case GameState::GlobalGameState::Score:
         {
            auto mesh = fontSystem.makeTextMesh("Score screen", state.fontDesc, {1, 1}, {Center, Middle}, Rect{{}, {200, 20}});
            systems.gfx->renderMesh({}, {}, mesh);
         } break;
      }

   }
}