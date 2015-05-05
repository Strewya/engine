//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "games/rainbowland/game_state.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "games/rainbowland/game_resources.h"
#include "games/rainbowland/game_systems.h"
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

namespace Core
{
   bool initializeGameState(Timer& timer, GameState& state, GameSystems& systems, GameResources& assets)
   {
      state.activeState = GameState::State::Session;

      state.cameraMoveModifier = 1;

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

      return true;
   }

   bool updateGameState(Timer& timer, GameState& state, GameSystems& systems, GameResources& assets)
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
      

      //input collection
      auto eventList = systems.input->getEvents();
      auto gameEvents = translateWindowEventsToGameEvents(eventList, state.inputTranslation);
      //input response
      modifyPlayerDirectionTargets(gameEvents, state.moveDirections);
      //pre-movement
      updatePlayerMovementDirection(timer.getDeltaSeconds(), state.moveDirections, state.movingThings);
      //movement
      simulateMovement(timer.getDeltaSeconds(), state.movingThings);

      return stillRunning;
   }

   void renderGameState(Timer& timer, GameState& state, GameSystems& systems, GameResources& assets)
   {
      systems.gfx->setTransparency(false);
      systems.gfx->renderMesh(Transform{}, Color{}, state.backgroundMesh);
      systems.gfx->setTransparency(true);
      systems.gfx->renderMesh(Transform{state.movingThings[0].position}, Color{}, state.playerMesh);
      systems.gfx->renderMesh(Transform{state.movingThings[0].position + state.movingThings[0].direction, {0.1f, 0.1f}}, Color{}, makeSolidQuad({}, {1, 1}, assets.mainVS, assets.mainPS));
   }
}