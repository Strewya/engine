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
   bool game_init(GameState& state, GameSystems systems, GameResources& assets)
   {
      state.globalGameState = GameState::GlobalGameState::MainMenu;
      state.gameplayState = GameState::GameplayState::ClassPick;

      state.fontDesc = loadFont(CORE_RESOURCE("Defs/font.font"), assets.font, assets.mainVS, assets.mainPS);

      return true;
   }

   bool game_update(float dt, uint32_t deltaMicros, GameState& state, GameSystems systems, GameResources& assets)
   {
      bool stillRunning = true;
      enum
      {
         STATE,
         ACTION,
      };

      const auto& frameEvents = systems.input->getEvents();
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
                     session_init(state.sessionState, systems, assets);
                  }
               } break;
               case GameState::GameplayState::Session:
               {
                  session_update(dt, deltaMicros, state.sessionState, systems, assets);

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

   void game_render(float dt, GameState& state, GameSystems systems, GameResources& assets)
   {
      systems.gfx->applyCamera(state.camera);
      /*
            systems.gfx->setPerspectiveProjection();
            systems.gfx->setTransparency(false);
            systems.gfx->renderMesh(Transform{}, Color{}, state.backgroundMesh);
            systems.gfx->setTransparency(true);
            systems.gfx->renderMesh(Transform{state.movingThings[0].position}, Color{}, state.playerMesh);
            systems.gfx->renderMesh(Transform{state.movingThings[0].position + state.movingThings[0].direction, {0.1f, 0.1f}}, Color{}, makeSolidQuad({}, {1, 1}, assets.mainVS, assets.mainPS));
      */

      systems.gfx->setOrthographicProjection();
      systems.gfx->setTransparency(true);

      switch( state.globalGameState )
      {
         case GameState::GlobalGameState::MainMenu:
         {
            auto mesh = systems.font->makeTextMesh("Main menu", state.fontDesc, {1, 1}, {Center, Middle});
            systems.gfx->renderMesh({}, {}, mesh);
         } break;
         case GameState::GlobalGameState::Gameplay:
         {
            auto mesh = systems.font->makeTextMesh("Gameplay", state.fontDesc, {1, 1}, {Center, Middle});
            systems.gfx->renderMesh({}, {}, mesh);
            switch( state.gameplayState )
            {
               case GameState::GameplayState::ClassPick:
               {
                  mesh = systems.font->makeTextMesh("Class picking", state.fontDesc, {1, 1}, {Center, Middle});
                  systems.gfx->renderMesh({vec2f{0, -40}}, {}, mesh);
               } break;
               case GameState::GameplayState::Session:
               {
                  mesh = systems.font->makeTextMesh("Session", state.fontDesc, {1, 1}, {Center, Middle});
                  systems.gfx->renderMesh({vec2f{0, -40}}, {}, mesh);
                  
                  session_render(dt, state.sessionState, systems, assets);
               } break;
            }
         } break;
         case GameState::GlobalGameState::Score:
         {
            auto mesh = systems.font->makeTextMesh("Score screen", state.fontDesc, {1, 1}, {Center, Middle});
            systems.gfx->renderMesh({}, {}, mesh);
         } break;
      }

   }
}