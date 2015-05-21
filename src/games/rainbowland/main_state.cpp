//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "games/rainbowland/main_state.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "games/rainbowland/resources.h"
#include "games/rainbowland/systems.h"
#include "games/rainbowland/logic/logic.h"
#include "graphics/graphics_system.h"
#include "input/input_system.h"
#include "input/keyboard.h"
#include "util/time/clock.h"
#include "util/time/timer.h"
#include "util/transform.h"
#include "util/color.h"
#include "window/window_event.h"
/******* end headers *******/

namespace core
{
   static FontDescriptor fd;

   bool initializeGameState(Timer& timer, GameState& state, GameSystems& systems, GameResources& assets)
   {
      state.globalGameState = GameState::GlobalGameState::MainMenu;
      state.gameplayState = GameState::GameplayState::ClassPick;

      state.cameraMoveModifier = 1;

      fd = loadFont(CORE_RESOURCE("Defs/font.font"), systems.gfx->textures.loadFromFile(CORE_RESOURCE("Textures/font_t.png")));
      fd.vshader = assets.mainVS;
      fd.pshader = assets.mainPS;

      auto backgroundTexture = systems.gfx->textures.getData(assets.background);
      auto ratio = (float)backgroundTexture.width / (float)backgroundTexture.height;
      state.backgroundMesh = makeTexturedQuad({}, {ratio * 15, 15}, assets.background, {}, {1, 1}, assets.mainVS, assets.mainPS);

      auto atlasTexture = systems.gfx->textures.getData(assets.atlas);
      auto x = 1795, y = 420, w = (3085 - 1795) / 5, h = (673 - 420);
      ratio = (float)w / (float)h;
      state.playerMesh = makeTexturedQuad({}, {ratio * 1, 1}, assets.atlas,
                                          vec2f{(float)x / (float)atlasTexture.width, (float)y / (float)atlasTexture.height},
                                          vec2f{(float)(x + w) / (float)atlasTexture.width, (float)(y + h) / (float)atlasTexture.height},
                                          assets.mainVS, assets.mainPS);
      state.transforms.emplace_back();
      auto& playerTransform = state.transforms.back();
      playerTransform.set({0, 0}, {0.5f, 0.5f}, 0);

      state.movingThings.emplace_back();
      auto& playerMover = state.movingThings.back();
      playerMover.acceleration = 0;
      playerMover.velocity.set(0, 0);
      playerMover.position.set(0, 0);
      playerMover.direction.set(1, 0);

      state.moveDirections.emplace_back();
      auto& playerDirectionTarget = state.moveDirections.back();
      playerDirectionTarget.objId = 0;
      playerDirectionTarget.direction.set(0, 0);

      state.aimDirections.emplace_back();
      auto& playerAimDirection = state.aimDirections.back();
      playerAimDirection.direction.set(1, 0);

      WindowEvent we{};
      GameEvent ge{};

      we.type = WE_KEYBOARDKEY;
      we.keyboard.firstTimeDown = true;
      we.keyboard.key.isDown = true;
      we.keyboard.key.id = Keyboard::W;
      ge.type = GE_DIRECTION_CHANGE;
      ge.directionChange.playerId = 0;
      ge.directionChange.direction.set(0, +1);
      state.inputTranslation.push_back({we, ge});
      //
      we.keyboard.key.id = Keyboard::S;
      ge.directionChange.direction.set(0, -1);
      state.inputTranslation.push_back({we, ge});
      //
      we.keyboard.key.id = Keyboard::A;
      ge.directionChange.direction.set(-1, 0);
      state.inputTranslation.push_back({we, ge});
      //
      we.keyboard.key.id = Keyboard::D;
      ge.directionChange.direction.set(+1, 0);
      state.inputTranslation.push_back({we, ge});
      //
      we.keyboard.firstTimeDown = false;
      we.keyboard.key.isDown = false;
      we.keyboard.key.id = Keyboard::W;
      ge.directionChange.direction.set(0, -1);
      state.inputTranslation.push_back({we, ge});
      //
      we.keyboard.key.id = Keyboard::S;
      ge.directionChange.direction.set(0, +1);
      state.inputTranslation.push_back({we, ge});
      //
      we.keyboard.key.id = Keyboard::A;
      ge.directionChange.direction.set(+1, 0);
      state.inputTranslation.push_back({we, ge});
//
we.keyboard.key.id = Keyboard::D;
ge.directionChange.direction.set(-1, 0);
state.inputTranslation.push_back({we, ge});
//
we.type = WE_MOUSEMOVE;


state.movement.allocate(8);

auto e = state.entities.create();
state.movement.make(e);

auto ec = state.movement.getComponentData(e);
float& eacc = state.movement.acceleration(ec);
eacc += 10;

state.movement.destroy(e);


return true;
   }

   // #delete after testing
   TextJustification justifyX = Left;
   TextJustification justifyY = Top;
   char axisToModify = ' ';
   int32_t justificationChange = -1;
   Rect box{{}, {100, 100}};
   bool fasterMod = false;

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

      auto it = std::find_if(eventList.begin(), eventList.end(), [](const WindowEvent& e)
      {
         return(e.type == WE_KEYBOARDKEY && e.keyboard.key.id == Keyboard::Shift);
      });
      if( it != eventList.end() )
      {
         if( it->keyboard.firstTimeDown )
         {
            fasterMod = true;
         }
         else if( it->keyboard.key.isDown == false )
         {
            fasterMod = false;
         }
      }
      
      float modSpeed = fasterMod ? 3.0f : 1.0f;

      if( contains(Keyboard::X, ACTION) )
      {
         axisToModify = 'x';
      }
      else if( contains(Keyboard::Y, ACTION) )
      {
         axisToModify = 'y';
      }
      else if( contains(Keyboard::Backspace, ACTION) )
      {
         axisToModify = ' ';
      }
      if( contains(Keyboard::_1, ACTION) )
      {
         if( axisToModify == 'x' )
         {
            justifyX = Left;
         }
         if( axisToModify == 'y' )
         {
            justifyY = Top;
         }
      }
      else if( contains(Keyboard::_2, ACTION) )
      {
         if( axisToModify == 'x' )
         {
            justifyX = Center;
         }
         if( axisToModify == 'y' )
         {
            justifyY = Middle;
         }
      }
      else if( contains(Keyboard::_3, ACTION) )
      {
         if( axisToModify == 'x' )
         {
            justifyX = Right;
         }
         if( axisToModify == 'y' )
         {
            justifyY = Bottom;
         }
      }
      else if( contains(Keyboard::NumSubtract, STATE) )
      {
         if( axisToModify == 'x' )
         {
            box.halfSize.x -= modSpeed;
         }
         if( axisToModify == 'y' )
         {
            box.halfSize.y -= modSpeed;
         }
      }
      else if( contains(Keyboard::NumAdd, STATE) )
      {
         if( axisToModify == 'x' )
         {
            box.halfSize.x += modSpeed;
         }
         if( axisToModify == 'y' )
         {
            box.halfSize.y += modSpeed;
         }
      }


      switch( state.globalGameState )
      {
         case GameState::GlobalGameState::MainMenu:
         {
            if( contains(Keyboard::M, ACTION) )
            {
               CORE_INFO("Switching to gameplay class pick state");
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
                  if( contains(Keyboard::P, ACTION) )
                  {
                     CORE_INFO("Switching to gameplay session state");
                     state.gameplayState = GameState::GameplayState::Session;
                  }
               } break;
               case GameState::GameplayState::Session:
               {
                  if( contains(Keyboard::S, ACTION) )
                  {
                     CORE_INFO("Switching to score state");
                     state.globalGameState = GameState::GlobalGameState::Score;
                  }
               } break;
            }
         } break;
         case GameState::GlobalGameState::Score:
         {
            if( contains(Keyboard::S, ACTION) )
            {
               CORE_INFO("Switching to main menu state");
               state.globalGameState = GameState::GlobalGameState::MainMenu;
            }
         } break;
      }


      /*
            //input collection
            auto eventList = systems.input->getEvents();
            auto gameEvents = translateWindowEventsToGameEvents(eventList, state.inputTranslation);
            //input response
            modifyPlayerDirectionTargets(gameEvents, state.moveDirections);
            //pre-movement
            updatePlayerMovementDirection(dt_s, state.moveDirections, state.movingThings);
            //movement
            simulateMovement(dt_s, state.movingThings);
            */

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

      systems.gfx->setOrthographicProjection();
      systems.gfx->setTransparency(true);

      auto boxMesh = makeOutlineQuad(box.center, box.halfSize, assets.mainVS, assets.mainPS);

      auto* text = "A somewhat larger amount of text so i can see if line breaks work properly. Also,\ntesting\nnew\nlines! a_really_long_word_that_doesnt_fit";

      vec2f pos{-200, -200};
      systems.gfx->renderMesh(pos, {}, boxMesh);
      systems.gfx->renderText(Transform{pos, {0.5f, 0.5f}, 0}, Color{}, text, fd, box, justifyX, justifyY);

      pos = {200, 200};
      systems.gfx->renderMesh(pos, {}, boxMesh);
      systems.gfx->renderText(Transform{pos, {1, 1}, 0}, Color{1, 0, 0}, text, fd, box, justifyX, justifyY);

      pos = {-200, 200};
      systems.gfx->renderMesh(pos, {}, boxMesh);
      systems.gfx->renderText(Transform{pos, {1.5f, 1.5f}, 0}, Color{1, 1, 0}, text, fd, box, justifyX, justifyY);
      /*

            switch( state.globalGameState )
            {
            case GameState::GlobalGameState::MainMenu:
            {
            systems.gfx->renderText("Main menu", fd, box, Left, Left);
            } break;
            case GameState::GlobalGameState::Gameplay:
            {
            systems.gfx->renderText("Gameplay", fd, box, Left, Left);
            box.halfSize.y -= 40;
            switch( state.gameplayState )
            {
            case GameState::GameplayState::ClassPick:
            {
            systems.gfx->renderText("Class pick", fd, box, Left, Left);
            } break;
            case GameState::GameplayState::Session:
            {
            systems.gfx->renderText("Session", fd, box, Left, Left);
            } break;
            }
            } break;
            case GameState::GlobalGameState::Score:
            {
            systems.gfx->renderText("Score", fd, box, Left, Left);
            } break;
            }*/

   }
}