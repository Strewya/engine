//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "games/rainbowland/game_state.h"
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
   bool game_init(GameState& state, GameSystems services, GameResources& assets)
   {
      state.globalGameState = GameState::GlobalGameState::MainMenu;
      state.gameplayState = GameState::GameplayState::ClassPick;

      state.fontDesc = loadFont(CORE_RESOURCE("Defs/font.font"), assets.font, assets.mainVS, assets.mainPS);

      return true;
   }

   bool game_update(float dt, uint32_t deltaMicros, GameState& state, GameSystems services, GameResources& assets)
   {
      bool stillRunning = true;
      enum
      {
         STATE,
         ACTION,
      };

      const auto& frameEvents = services.input->getEvents();
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

   void game_render(float dt, GameState& state, GameSystems services, GameResources& assets)
   {
      services.gfx->applyCamera(state.camera);
      /*
            systems.gfx->setPerspectiveProjection();
            systems.gfx->setTransparency(false);
            systems.gfx->renderMesh(Transform{}, Color{}, state.backgroundMesh);
            systems.gfx->setTransparency(true);
            systems.gfx->renderMesh(Transform{state.movingThings[0].position}, Color{}, state.playerMesh);
            systems.gfx->renderMesh(Transform{state.movingThings[0].position + state.movingThings[0].direction, {0.1f, 0.1f}}, Color{}, makeSolidQuad({}, {1, 1}, assets.mainVS, assets.mainPS));
      */

      services.gfx->setOrthographicProjection();
      services.gfx->setTransparency(true);

      switch( state.globalGameState )
      {
         case GameState::GlobalGameState::MainMenu:
         {
            auto mesh = services.font->makeTextMesh("Main menu", state.fontDesc, {1, 1}, Center, Middle);
            services.gfx->renderMesh({}, {}, mesh);
         } break;
         case GameState::GlobalGameState::Gameplay:
         {
            auto mesh = services.font->makeTextMesh("Gameplay", state.fontDesc, {1, 1}, Center, Middle);
            services.gfx->renderMesh({}, {}, mesh);
            switch( state.gameplayState )
            {
               case GameState::GameplayState::ClassPick:
               {
                  mesh = services.font->makeTextMesh("Class picking", state.fontDesc, {1, 1}, Center, Middle);
                  services.gfx->renderMesh({Vec2{0, -40}}, {}, mesh);
               } break;
               case GameState::GameplayState::Session:
               {
                  mesh = services.font->makeTextMesh("Session", state.fontDesc, {1, 1}, Center, Middle);
                  services.gfx->renderMesh({Vec2{0, -40}}, {}, mesh);
               } break;
            }
         } break;
         case GameState::GlobalGameState::Score:
         {
            auto mesh = services.font->makeTextMesh("Score screen", state.fontDesc, {1, 1}, Center, Middle);
            services.gfx->renderMesh({}, {}, mesh);
         } break;
      }

   }
}