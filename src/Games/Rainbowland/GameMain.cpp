//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Rainbowland/GameDefinitions.h>
/******* C++ headers *******/
#include <cstdint>
#include <fstream>
#include <map>
/******* extra headers *******/
#include <Games/Rainbowland/BonusDatabase.h>
#include <Games/Rainbowland/InputBindings.h>
#include <Games/Rainbowland/PerkDatabase.h>
#include <Games/Rainbowland/Rainbowland.h>
#include <Games/Rainbowland/SessionPreparation.h>
#include <Games/Rainbowland/WeaponDatabase.h>
#include <Graphics/Camera.h>
#include <Graphics/GraphicsSystem.h>
#include <Util/CollisionChecks.h>
#include <Util/Random.h>
#include <Util/Rect.h>
#include <Util/Time.h>
#include <Util/Utility.h>
#include <Window/Window.h>
/******* end headers *******/

namespace Core
{
   void initGridDatabase(float startX, float startY,
      float width, float height,
      uint32_t columns, uint32_t rows,
      uint32_t totalCount,
      std::vector<Rect>& out)
   {
      float w = width / columns;
      float h = height / rows;
      float hw = w * 0.5f;
      float hh = h * 0.5f;
      for(uint32_t y = 0; y < rows; ++y)
      {
         for(uint32_t x = 0; x < columns; ++x)
         {
            out.emplace_back(Rect{Vec2{startX + x*w + hw, startY + y*h + hh}, hw, hh});
            if(totalCount > 0)
            {
               --totalCount;
            }
            if(totalCount == 0)
            {
               return;
            }
         }
      }
   }

   void initGame(RainbowlandGame& game)
   {
      if(game.m_isRunning)
      {
         game.m_messageHandlers.emplace_back([&](const WindowEvent& w)
         {
            if ((w.m_type == WindowEventType::WE_KEYBOARDKEY && w.m_keyboard.m_keyCode == Keyboard::m_Escape && w.m_keyboard.m_isDown && !w.m_keyboard.m_previouslyDown) )
               //(w.m_type == WindowEventType::WE_GAMEPADBUTTON && w.m_gamepadButton.m_button == Gamepad::m_LeftShoulder && w.m_gamepadButton.m_isDown))
            {
               if (game.m_currentGameState == RainbowlandGame::GS_SessionPreparation)
               {
                  game.m_isRunning = false;
                  return true;
               }
               else if (game.m_currentGameState == RainbowlandGame::GS_Session)
               {
                  cleanSession(game);
                  game.m_nextGameState = RainbowlandGame::GS_SessionPreparation;
                  return true;
               }
            }
            return false;
         });

#ifndef DEPLOY
         setupDebugInputBindings(game);
#endif

         game.m_isRunning &= game.m_graphicsSystem.initVertexShader("Shaders/shader.hlsl");
         game.m_isRunning &= game.m_graphicsSystem.initPixelShader("Shaders/shader.hlsl");

         game.m_isRunning &= (bool)game.m_textureCache.load("Textures/font_t.png");
         game.m_isRunning &= (bool)game.m_textureCache.load("Textures/background.png");
         game.m_isRunning &= (bool)game.m_textureCache.load("Textures/rainbowland_atlas.png");
         game.m_isRunning &= (bool)game.m_fontCache.load("Defs/font.font", game.m_luaSystem.getStack());

         game.m_shootSfx = game.m_audioSystem.loadSound("Sounds/shot.mp3");
         game.m_music = game.m_audioSystem.loadSound("Sounds/music.mp3");
         game.m_reloadSfx = game.m_audioSystem.loadSound("Sounds/reload.wav");
         game.m_isRunning &= (game.m_music != nullptr && game.m_shootSfx != nullptr && game.m_reloadSfx != nullptr);
      }

      if(game.m_isRunning)
      {
         game.m_window->resize(CW_USEDEFAULT, CW_USEDEFAULT);
         game.m_window->lockCursor(true);
         game.m_window->showCursor(false);

         game.m_defaultFont = game.m_fontCache.getResourceID("font");
         game.m_backgroundTexture = game.m_textureCache.getResourceID("Textures/background.png");
         game.m_atlasTexture = game.m_textureCache.getResourceID("Textures/rainbowland_atlas.png");

         game.m_imageStartIndex_flower = game.m_rainbowlandImageDatabase.size();
         initGridDatabase(0, 0, 1312, 422, 4, 2, 7, game.m_rainbowlandImageDatabase);

         game.m_imageStartIndex_ladybug = game.m_rainbowlandImageDatabase.size();
         initGridDatabase(0, 422, 481, 431, 3, 3, 7, game.m_rainbowlandImageDatabase);

         game.m_imageStartIndex_splatter = game.m_rainbowlandImageDatabase.size();
         initGridDatabase(0, 853, 1125, 900, 5, 6, 5 * 6, game.m_rainbowlandImageDatabase);

         game.m_imageStartIndex_butterfly = game.m_rainbowlandImageDatabase.size();
         initGridDatabase(481, 422, 1220, 251, 7, 1, 7, game.m_rainbowlandImageDatabase);

         game.m_imageStartIndex_blast = game.m_rainbowlandImageDatabase.size();
         initGridDatabase(1179, 673, 884, 885, 1, 1, 1, game.m_rainbowlandImageDatabase);

         game.m_imageStartIndex_crosshair = game.m_rainbowlandImageDatabase.size();
         initGridDatabase(2063, 673, 440, 443, 1, 1, 1, game.m_rainbowlandImageDatabase);

         game.m_imageStartIndex_bonuses = game.m_rainbowlandImageDatabase.size();
         initGridDatabase(1353, 0, 2893 - 1353, 420, 11, 3, 33, game.m_rainbowlandImageDatabase);

         game.m_imageStartIndex_players = game.m_rainbowlandImageDatabase.size();
         initGridDatabase(1795, 420, 3085 - 1795, 673 - 420, 5, 1, 5, game.m_rainbowlandImageDatabase);

         game.m_imageStartIndex_rocket = game.m_rainbowlandImageDatabase.size();
         initGridDatabase(2538, 673, 2613 - 2538, 884 - 673, 1, 1, 1, game.m_rainbowlandImageDatabase);

         game.m_imageStartIndex_turret = game.m_rainbowlandImageDatabase.size();
         initGridDatabase(2087, 1178, 2377 - 2087, 1470 - 1178, 1, 1, 1, game.m_rainbowlandImageDatabase);

         game.m_playerCount = 0;
         game.m_nextGameState = game.m_currentGameState = RainbowlandGame::GS_SessionPreparation;

         game.m_randomGenerator.reseed(Time::getRealTimeMicros());

         game.m_graphicsSystem.setBackgroundColor(0.5f, 0.5f, 0.5f);
         //playing field boundary
         game.m_graphicsSystem.setPerspectiveProjection();


         game.m_cameraBounds.set(3, 3);
         game.m_camera.setPosition({-game.m_cameraBounds.x, game.m_cameraBounds.y, -50.0f});
         game.m_graphicsSystem.applyCamera(game.m_camera);
         Vec2 topleft = game.m_graphicsSystem.screenToWorld({0, 0}, game.m_camera);
         game.m_playingField.halfWidth = std::abs(topleft.x);
         game.m_playingField.halfHeight = std::abs(topleft.y);

         game.m_camera.setPosition({0, 0, -50});
         game.m_graphicsSystem.setTransparencyMode(true);

//         game.m_audioSystem.playMusic(game.m_music);

         //spawners
         Rect spawnerLocations{{0, 0}, game.m_playingField.halfWidth + 3, game.m_playingField.halfHeight + 3};
         uint32_t spawnerCount = 8;
         Vec2 distanceBetweenSpawners{spawnerLocations.halfWidth * 2 / spawnerCount, spawnerLocations.halfHeight * 2 / spawnerCount};
         Vec2 pos{spawnerLocations.left(), spawnerLocations.top()};
         for(uint32_t i = 0; i < spawnerCount; ++i)
         {
            game.m_monsterSpawners.emplace_back();
            game.m_monsterSpawners.back().spawnRadius = 1;
            game.m_monsterSpawners.back().transform.position = pos;

            pos.x += distanceBetweenSpawners.x;
         }
         for(uint32_t i = 0; i < spawnerCount; ++i)
         {
            game.m_monsterSpawners.emplace_back();
            game.m_monsterSpawners.back().spawnRadius = 1;
            game.m_monsterSpawners.back().transform.position = pos;

            pos.y -= distanceBetweenSpawners.y;
         }
         for(uint32_t i = 0; i < spawnerCount; ++i)
         {
            game.m_monsterSpawners.emplace_back();
            game.m_monsterSpawners.back().spawnRadius = 1;
            game.m_monsterSpawners.back().transform.position = pos;

            pos.x -= distanceBetweenSpawners.x;
         }
         for(uint32_t i = 0; i < spawnerCount; ++i)
         {
            game.m_monsterSpawners.emplace_back();
            game.m_monsterSpawners.back().spawnRadius = 1;
            game.m_monsterSpawners.back().transform.position = pos;

            pos.y += distanceBetweenSpawners.y;
         }

         initWeaponDatabase(game.m_weaponDatabase);
         initPerkDatabase(game.m_perkDatabase);
         initBonusDatabase(game.m_bonusDatabase);

         game.m_deathTimer.setDurationMicros(5 * 1000000U);
         game.m_defenseMatrixPlaying = false;
         game.m_timeCapsulePlaying = false;
         game.m_blinkPlaying = false;
         game.m_turretPlaying = false;
         game.m_difficultyTimer.setPeriodMicros(5 * 1000000U);
         game.m_perkModeTransitionTimer.setDurationMicros(2*1000000U);
         game.m_perkModeTransitionTimer.reset();
         game.m_enteringPerkMode = false;
         game.m_exitingPerkMode = false;
         game.m_highScore = 0;

         std::ifstream file("score", std::ios::in);
         if (file.is_open())
         {
            file >> game.m_highScore;
         }
         file.close();

         initPreparationHandlers(game);

         game.m_preparationData.start = false;
         for(uint32_t i = 0; i < 4; ++i)
         {
            game.m_preparationData.classes[i] = -1;
         }
         for(uint32_t i = 0; i < 5; ++i)
         {
            game.m_preparationData.controllers[i] = -1;
         }
      }
   }



   void cleanSession(RainbowlandGame& game)
   {
      game.m_players.clear();
      game.m_deadPlayers.clear();
      game.m_monsters.clear();
      game.m_pickups.clear();
      game.m_bullets.clear();
      game.m_blasts.clear();
      game.m_splatters.clear();
      for (auto& bonus : game.m_activeBonuses)
      {
         game.m_bonusDatabase[bonus.type].timeoutLogic(game);
      }
      game.m_activeBonuses.clear();
      game.m_graphicsSystem.clearTextureRenderTarget();
      game.m_preparationData.start = false;
      for(uint32_t i = 0; i < 4; ++i)
      {
         game.m_preparationData.classes[i] = -1;
      }
      for(uint32_t i = 0; i < 5; ++i)
      {
         game.m_preparationData.controllers[i] = -1;
      }
      game.m_sessionHandlers.clear();
      game.m_sessionPerkMenuHandlers.clear();
      game.m_defenseMatrixPlaying = false;
      game.m_timeCapsulePlaying = false;
      game.m_blinkPlaying = false;
      game.m_turretPlaying = false;
   }
}
