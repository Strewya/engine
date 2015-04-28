//headers should be ordered alphabetically!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <games/rainbowland/game_state.h>
/******* c++ headers *******/
/******* extra headers *******/
#include <games/rainbowland/game_resources.h>
#include <games/rainbowland/game_systems.h>
#include <graphics/graphics_system.h>
#include <util/transform.h>
#include <util/color.h>
/******* end headers *******/

namespace Core
{
   void initializeSession(GameState& state, GameSystems& systems, GameResources& assets)
   {

   }

   void initializeGameState(GameState& state, GameSystems& systems, GameResources& assets)
   {
      state.activeState = GameState::State::Session;

      state.cameraMoveModifier = 1;

      auto backgroundTexture = systems.gfx->textures.getData(assets.background);
      auto ratio = (float)backgroundTexture.width / (float)backgroundTexture.height;
      state.background = makeTexturedQuad({}, {ratio*15, 15}, assets.background, {}, {1, 1}, assets.mainVS, assets.mainPS);

      auto atlasTexture = systems.gfx->textures.getData(assets.atlas);
      auto x = 1795, y = 420, w = (3085 - 1795) / 5, h = (673 - 420);
      ratio = (float)w / (float)h;
      state.player = makeTexturedQuad({}, {ratio * 1, 1}, assets.atlas,
                                      Vec2f{(float)x / (float)atlasTexture.width, (float)y / (float)atlasTexture.height},
                                      Vec2f{(float)(x + w) / (float)atlasTexture.width, (float)(y + h) / (float)atlasTexture.height},
                                      assets.mainVS, assets.mainPS);

      switch( state.activeState )
      {
         case GameState::State::Session:
         {
            initializeSession(state, systems, assets);
         } break;
      }

   }

   void updateGameState(GameState& state, GameSystems& systems, GameResources& assets)
   {
      switch( state.activeState )
      {
         case GameState::State::Session:
         {
            
         } break;
         case GameState::State::ClassPick:
         {

         } break;
         case GameState::State::ColorPick:
         {

         } break;
      }
   }

   void renderGameState(GameState& state, GameSystems& systems, GameResources& assets)
   {
      systems.gfx->renderer.setTransparency(false);
      systems.gfx->renderMesh(Transform{}, Color{}, state.background);
      systems.gfx->renderer.setTransparency(true);
      systems.gfx->renderMesh(Transform{}, Color{}, state.player);
   }
}