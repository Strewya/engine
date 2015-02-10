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
            out.emplace_back(Rect{Vec2f{startX + x*w + hw, startY + y*h + hh}, hw, hh});
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
         //game.m_window->resize(CW_USEDEFAULT, CW_USEDEFAULT);
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

         //all non-changeable values first
         game.m_randomGenerator.reseed(Clock::getRealTimeMicros());

         //playing field calculation
         game.m_graphicsSystem.setPerspectiveProjection();
         game.m_cameraBounds.set(3, 3);
         game.m_camera.setPosition({-game.m_cameraBounds.x, game.m_cameraBounds.y, -50.0f});
         game.m_graphicsSystem.applyCamera(game.m_camera);
         Vec2f topleft = game.m_graphicsSystem.screenToWorld({0, 0}, game.m_camera);
         game.m_playingField.halfWidth = std::abs(topleft.x);
         game.m_playingField.halfHeight = std::abs(topleft.y);
         game.m_camera.setPosition({0, 0, -50});

         //spawners
         Rect spawnerLocations{{0, 0}, game.m_playingField.halfWidth + 3, game.m_playingField.halfHeight + 3};
         uint32_t spawnerCount = 8;
         Vec2f distanceBetweenSpawners{spawnerLocations.halfWidth * 2 / spawnerCount, spawnerLocations.halfHeight * 2 / spawnerCount};
         Vec2f pos{spawnerLocations.left(), spawnerLocations.top()};
         for(uint32_t i = 0; i < spawnerCount*4; ++i)
         {
            game.m_monsterSpawners.emplace_back();
            game.m_monsterSpawners.back().spawnRadius = 1;
            game.m_monsterSpawners.back().transform.position = pos;
            switch( i / spawnerCount )
            {
               case 0:
               {
                  pos.x += distanceBetweenSpawners.x;
               } break;
               case 1:
               {
                  pos.y -= distanceBetweenSpawners.y;
               } break;
               case 2:
               {
                  pos.x -= distanceBetweenSpawners.x;
               } break;
               case 3:
               {
                  pos.y += distanceBetweenSpawners.y;
               } break;
            }
         }

         //databases
         initWeaponDatabase(game.m_weaponDatabase);
         initPerkDatabase(game.m_perkDatabase);
         initBonusDatabase(game.m_bonusDatabase);

         //timers
         game.m_deathTimer.setDurationMicros(secondsToMicros(5U));
         game.m_difficultyTimer.setPeriodMicros(secondsToMicros(5U));
         game.m_perkModeTransitionTimer.setDurationMicros(secondsToMicros(2U));

         game.m_perkModeTransitionTimer.reset();
         game.m_enteringPerkMode = false;
         game.m_exitingPerkMode = false;
         game.m_highScore = 0;
         game.m_maxMonsterCount = 400;
         game.m_monsters.reserve(game.m_maxMonsterCount);

         //high score
         game.m_highScore = 0;
         std::ifstream file("score", std::ios::in);
         if (file.is_open())
         {
            file >> game.m_highScore;
         }
         file.close();

         //abilities
         game.m_defenseMatrix.area.radius = 4;
         game.m_defenseMatrix.durationSeconds = 8;

         game.m_timeCapsule.area.radius = 10;
         game.m_timeCapsule.durationSeconds = 8;
         game.m_timeCapsule.maxHealPeriod = 40;

         game.m_blink.area.radius = 6;
         game.m_blink.durationSeconds = 0.1f;

         game.m_turret.area.radius = 1;
         game.m_turret.durationSeconds = 8;

         //finally, prepare for game start
         initPreparationHandlers(game); //i will say this only once

         game.m_preparationData.start = false;
         memset(game.m_preparationData.classes, -1, sizeof(int32_t) * 4);
         memset(game.m_preparationData.controllers, -1, sizeof(int32_t) * 5);

         game.m_nextGameState = game.m_currentGameState = RainbowlandGame::GS_SessionPreparation;
         
         //game.m_audioSystem.playMusic(game.m_music);
      }
   }

   void initSession(RainbowlandGame& game)
   {
      game.m_graphicsSystem.createTextureRenderTarget(1200, 900);

      game.m_deathTimer.reset();
      game.m_currentTimeScale = 1;
      game.m_gameplayTimer.setTimeScale(game.m_currentTimeScale);
      game.m_difficulty = 1;
      game.m_killCounter = 0;
      game.m_totalKillCount = 0;
      game.m_difficultyTimer.hardReset();
      game.m_flavour = -1;

      game.m_defenseMatrix.active = false;
      game.m_defenseMatrix.timer.setDurationMicros(0);
      game.m_defenseMatrix.timer.reset();
      game.m_defenseMatrix.cooldownSeconds = 20;
      game.m_timeCapsule.active = false;
      game.m_timeCapsule.timer.setDurationMicros(0);
      game.m_timeCapsule.timer.reset();
      game.m_timeCapsule.cooldownSeconds = 20;
      
      game.m_blink.active = false;
      game.m_blink.timer.setDurationMicros(0);
      game.m_blink.timer.reset();
      game.m_blink.cooldownSeconds = 20;
      
      game.m_turret.active = false;
      game.m_turret.timer.setDurationMicros(0);
      game.m_turret.timer.reset();
      game.m_turret.killCount = 0;
      game.m_turret.cooldownSeconds = 20;

      game.m_experience = 0;
      game.m_experienceIncrement = 5000 * game.m_playerCount;
      game.m_experienceForNextLevel = 5000;
      game.m_level = 1;

      for( auto& spawner : game.m_monsterSpawners )
      {
         spawner.spawnTimer.hardReset();
         spawner.spawnTimer.setPeriodMicros(
            secondsToMicros(game.m_randomGenerator.randFloat() * 5 + 1) / game.m_playerCount);
      }

      game.m_players.reserve(game.m_playerCount);
      for( auto soldier = 0; soldier < 4; ++soldier )
      {
         auto controller = game.m_preparationData.classes[soldier];
         if( controller != -1 )
         {
            game.m_players.emplace_back();
            initPlayer(game.m_players.back(), soldier, game);
            if( controller == 0 )
            {
               //keyboard
               registerKeyboardMouse(game, soldier);
            }
            else
            {
               //gamepad
               registerGamepad(game, soldier, controller - 1);
            }
         }
      }

      game.m_monsterGrid.cellHalfsize.set(2, 2);
      game.m_monsterGrid.columns = (uint32_t)(game.m_playingField.halfWidth * 2 / game.m_monsterGrid.cellHalfsize.x);
      game.m_monsterGrid.rows = (uint32_t)(game.m_playingField.halfHeight * 2 / game.m_monsterGrid.cellHalfsize.y);
      game.m_monsterGrid.cells.resize(game.m_monsterGrid.columns*game.m_monsterGrid.rows);
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
         disableBonus(bonus.type, game);
      }
      game.m_activeBonuses.clear();
      game.m_graphicsSystem.clearTextureRenderTarget();
      game.m_preparationData.start = false;
      memset(game.m_preparationData.classes, -1, sizeof(int32_t) * 4);
      memset(game.m_preparationData.controllers, -1, sizeof(int32_t) * 5);
      game.m_sessionHandlers.clear();
      game.m_sessionPerkMenuHandlers.clear();
   }
}
