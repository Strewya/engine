//headers should be ordered alphabetically!
/******* precompiled header *******/
#ifdef CORE_USE_PCH
#undef CORE_USE_PCH
#include "stdafx.h"
#define CORE_USE_PCH
#endif
/******* personal header *******/
#include "games/rainbowland/logic/code.h"
/******* c++ headers *******/
/******* extra headers *******/

/******* end headers *******/

namespace core
{
   bool game_init(GameState& state, GameResources& assets)
   {
      state.globalGameState = GameState::GlobalGameState::MainMenu;
      state.gameplayState = GameState::GameplayState::ClassPick;

      state.fontDesc = loadFont(CORE_RESOURCE("Defs/font.font"), assets.font, assets.mainVS, assets.mainPS);

      return true;
   }

   bool game_update(Time time, GameState game, const EventVector_t& frameEvents, GameResources assets, AudioSystem& audio, LuaStack lua)
   {
      bool stillRunning = true;
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

      switch( game.globalGameState )
      {
         case GameState::GlobalGameState::MainMenu:
         {
            if( contains(Keyboard::Space, ACTION) )
            {
               game.globalGameState = GameState::GlobalGameState::Gameplay;
               game.gameplayState = GameState::GameplayState::ClassPick;
            }
         } break;
         case GameState::GlobalGameState::Gameplay:
         {
            switch( game.gameplayState )
            {
               case GameState::GameplayState::ClassPick:
               {
                  if( contains(Keyboard::Space, ACTION) )
                  {
                     game.gameplayState = GameState::GameplayState::Session;
                  }
               } break;
               case GameState::GameplayState::Session:
               {
                  if( contains(Keyboard::Space, ACTION) )
                  {
                     game.globalGameState = GameState::GlobalGameState::Score;
                  }
               } break;
            }
         } break;
         case GameState::GlobalGameState::Score:
         {
            if( contains(Keyboard::Space, ACTION) )
            {
               game.globalGameState = GameState::GlobalGameState::MainMenu;
            }
         } break;
      }

      return stillRunning;
   }

   void game_render(Time time, GameState& game, GameResources assets, GraphicsSystem& gfx, FontSystem& font)
   {
      gfx.applyCamera(game.camera);
      /*
      systems.gfx->setPerspectiveProjection();
      systems.gfx->setTransparency(false);
      systems.gfx->renderMesh(Transform{}, Color{}, state.backgroundMesh);
      systems.gfx->setTransparency(true);
      systems.gfx->renderMesh(Transform{state.movingThings[0].position}, Color{}, state.playerMesh);
      systems.gfx->renderMesh(Transform{state.movingThings[0].position + state.movingThings[0].direction, {0.1f, 0.1f}}, Color{}, makeSolidQuad({}, {1, 1}, assets.mainVS, assets.mainPS));
      */

      gfx.setOrthographicProjection();
      gfx.setTransparency(true);

      switch( game.globalGameState )
      {
         case GameState::GlobalGameState::MainMenu:
         {
            auto mesh = font.makeTextMesh("Main menu", game.fontDesc, {1, 1}, Center, Middle);
            gfx.renderMesh({}, {}, mesh);
         } break;
         case GameState::GlobalGameState::Gameplay:
         {
            auto mesh = font.makeTextMesh("Gameplay", game.fontDesc, {1, 1}, Center, Middle);
            gfx.renderMesh({}, {}, mesh);
            switch( game.gameplayState )
            {
               case GameState::GameplayState::ClassPick:
               {
                  mesh = font.makeTextMesh("Class picking", game.fontDesc, {1, 1}, Center, Middle);
                  gfx.renderMesh({Vec2{0, -40}}, {}, mesh);
               } break;
               case GameState::GameplayState::Session:
               {
                  mesh = font.makeTextMesh("Session", game.fontDesc, {1, 1}, Center, Middle);
                  gfx.renderMesh({Vec2{0, -40}}, {}, mesh);
               } break;
            }
         } break;
         case GameState::GlobalGameState::Score:
         {
            auto mesh = font.makeTextMesh("Score screen", game.fontDesc, {1, 1}, Center, Middle);
            gfx.renderMesh({}, {}, mesh);
         } break;
      }

   }











   /*void updatePlayerMovementDirection(float dt, VectorOfMoveDirectionTargets& directions, VectorOfMovables& movables)
   {
      for( auto& dir : directions )
      {
         auto& mover = movables[dir.objId];
         mover.acceleration = 60.0f*(vec2::length2(dir.direction) > 0.0f ? 1 : 0);
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
   }*/
}