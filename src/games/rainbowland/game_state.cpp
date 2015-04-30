//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "games/rainbowland/game_state.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "games/rainbowland/game_resources.h"
#include "games/rainbowland/game_systems.h"
#include "graphics/graphics_system.h"
#include "input/input_system.h"
#include "input/keyboard.h"
#include "util/clock.h"
#include "util/timer.h"
#include "util/transform.h"
#include "util/color.h"
#include "window/window_event.h"
/******* end headers *******/

namespace Core
{
   bool initializeGameState(Timer& timer, GameState& state, GameSystems& systems, GameResources& assets)
   {
      state.activeState = GameState::State::Session;

      state.cameraMoveModifier = 1;

      auto backgroundTexture = systems.gfx->textures.getData(assets.background);
      auto ratio = (float)backgroundTexture.width / (float)backgroundTexture.height;
      state.backgroundMesh = makeTexturedQuad({}, {ratio*15, 15}, assets.background, {}, {1, 1}, assets.mainVS, assets.mainPS);

      auto atlasTexture = systems.gfx->textures.getData(assets.atlas);
      auto x = 1795, y = 420, w = (3085 - 1795) / 5, h = (673 - 420);
      ratio = (float)w / (float)h;
      state.playerMesh = makeTexturedQuad({}, {ratio * 1, 1}, assets.atlas,
                                      Vec2f{(float)x / (float)atlasTexture.width, (float)y / (float)atlasTexture.height},
                                      Vec2f{(float)(x + w) / (float)atlasTexture.width, (float)(y + h) / (float)atlasTexture.height},
                                      assets.mainVS, assets.mainPS);
      state.playerMover.acceleration = 1;
      state.playerMover.maxSpeed = 5;
      state.playerMover.currentSpeed = 0;
      state.playerMover.position.set(0, 0);
      state.playerMover.direction.set(0, 0);

      return true;
   }

   bool updateGameState(Timer& timer, GameState& state, GameSystems& systems, GameResources& assets)
   {
      bool stillRunning = true;
      
      auto eventList = systems.input->getEvents();
      enum Direction
      {
         Up,Down,Left,Right,Count
      };
      bool moveState[Direction::Count]{};
      struct KeyMapping
      {
         uint32_t key;
         float add;
         float* value;
      };
      KeyMapping map[4]
      {
         {Keyboard::W, -1, &state.playerMover.direction.y},
         {Keyboard::S, +1, &state.playerMover.direction.y},
         {Keyboard::A, -1, &state.playerMover.direction.x},
         {Keyboard::D, +1, &state.playerMover.direction.x}
      };
      for( auto& e : eventList )
      {
         if( e.type == WindowEventType::WE_KEYBOARDKEY )
         {
            for( auto& check : map )
            {
               if( e.keyboard.key.id == check.key )
               {
                  if( e.keyboard.firstTimeDown )
                  {
                     *check.value += check.add;
                  }
                  else if( e.keyboard.key.isDown )
                  {
                     *check.value -= check.add;
                  }
               }
            }
         }
      }

      //movement
      state.playerMover.currentSpeed += state.playerMover.acceleration*timer.getDeltaSeconds();
      clamp(state.playerMover.currentSpeed, 0.0f, state.playerMover.maxSpeed);
      state.playerMover.position += state.playerMover.direction*state.playerMover.currentSpeed*timer.getDeltaSeconds();

      return stillRunning;
   }

   void renderGameState(Timer& timer, GameState& state, GameSystems& systems, GameResources& assets)
   {
      systems.gfx->renderer.setTransparency(false);
      systems.gfx->renderMesh(Transform{}, Color{}, state.backgroundMesh);
      systems.gfx->renderer.setTransparency(true);
      systems.gfx->renderMesh(Transform{state.playerMover.position}, Color{}, state.playerMesh);
   }
}