//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Rainbowland/Session.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Games/Rainbowland/ObjectRendering.h>
#include <Games/Rainbowland/Rainbowland.h>
#include <Window/Window.h>
/******* end headers *******/

namespace Core
{
#ifndef DEPLOY
   bool g_spawnEnabled = true;
   bool g_step = false;
   bool g_stepBack = false;
   double g_debugPauseTime = 0;
   uint32_t g_mx = 0;
   uint32_t g_my = 0;
#endif

   void logic_session(RainbowlandGame& game)
   {
      auto evs = game.m_inputSystem.getEvents();
      for( auto& e : evs )
      {
         for( auto& f : game.m_messageHandlers )
         {
            if( f(e) )
            {
               continue;
            }
         }
         for( auto& f : game.m_sessionHandlers )
         {
            if( f(e) )
            {
               continue;
            }
         }
      }

#ifndef DEPLOY
      if( g_step )
      {
         auto currentScale = game.m_gameplayTimer.getTimeScale();
         game.m_gameplayTimer.setTimeScale(g_debugPauseTime);
         g_debugPauseTime = currentScale;
      }
      if( g_stepBack )
      {
         auto currentScale = game.m_gameplayTimer.getTimeScale();
         game.m_gameplayTimer.setTimeScale(-g_debugPauseTime);
         g_debugPauseTime = currentScale;
      }
#endif
      game.m_gameplayTimer.updateBy(game.m_logicTimer.getDeltaMicros());
#ifndef DEPLOY
      if( g_step )
      {
         g_step = false;
         auto currentScale = game.m_gameplayTimer.getTimeScale();
         game.m_gameplayTimer.setTimeScale(g_debugPauseTime);
         g_debugPauseTime = currentScale;
      }
      if( g_stepBack )
      {
         g_stepBack = false;
         auto currentScale = -game.m_gameplayTimer.getTimeScale();
         game.m_gameplayTimer.setTimeScale(g_debugPauseTime);
         g_debugPauseTime = currentScale;
      }
#endif

      if (game.m_enteringPerkMode)
      {
         game.m_perkModeTransitionTimer.updateBy(game.m_logicTimer.getDeltaMicros());
         if ( !game.m_perkModeTransitionTimer.hasElapsed() )
         {
            auto percent = game.m_perkModeTransitionTimer.getPercentDone();
            percent *= (float)game.m_currentTimeScale;
            game.m_gameplayTimer.setTimeScale(game.m_currentTimeScale - percent);
         }
         else
         {
            game.m_enteringPerkMode = false;
            game.m_perkModeTransitionTimer.reset();
            game.m_gameplayTimer.setTimeScale(0);
            game.m_nextGameState = RainbowlandGame::GS_SessionPerkMenu;
            for (auto& player : game.m_players)
            {
               player.directionActive[Up] = player.directionActive[Right] = player.directionActive[Left] = player.directionActive[Down] = false;
            }
         }
      }
      if (game.m_exitingPerkMode)
      {
         game.m_perkModeTransitionTimer.updateBy(game.m_logicTimer.getDeltaMicros());
         if ( !game.m_perkModeTransitionTimer.hasElapsed() )
         {
            auto percent = game.m_perkModeTransitionTimer.getPercentDone();
            percent *= (float)game.m_currentTimeScale;
            game.m_gameplayTimer.setTimeScale(percent);
         }
         else
         {
            game.m_exitingPerkMode = false;
            game.m_perkModeTransitionTimer.reset();
            game.m_gameplayTimer.setTimeScale(game.m_currentTimeScale);
            return;
         }
      }

      for( auto& obj : game.m_players )
      {
         obj.objectTimer.updateBy(game.m_gameplayTimer.getDeltaMicros());
      }
      for( auto* obj : game.m_monsters )
      {
         obj->objectTimer.updateBy(game.m_gameplayTimer.getDeltaMicros());
      }
      for( auto& obj : game.m_monsterSpawners )
      {
         obj.spawnTimer.updateBy(game.m_gameplayTimer.getDeltaMicros());
      }
      for( auto& obj : game.m_bullets )
      {
         obj.objectTimer.updateBy(game.m_gameplayTimer.getDeltaMicros());
      }
      for( auto& obj : game.m_blasts )
      {
         obj.objectTimer.updateBy(game.m_gameplayTimer.getDeltaMicros());
      }
      for( auto& obj : game.m_rockets )
      {
         obj.objectTimer.updateBy(game.m_gameplayTimer.getDeltaMicros());
      }
      for( auto& obj : game.m_pickups )
      {
         obj.objectTimer.updateBy(game.m_gameplayTimer.getDeltaMicros());
      }
      for( auto& obj : game.m_activeBonuses )
      {
         obj.timer.updateBy(game.m_gameplayTimer.getDeltaMicros());
      }

      updateDifficulty(game);
#ifndef DEPLOY
      if( g_spawnEnabled )
#endif
      {
         updateMonsterSpawners(game);
      }
      runMonsterAI(game);

      movePlayers(game);
      moveMonsters(game);
      moveBullets(game.m_bullets);
      moveBlasts(game.m_blasts);
      moveRockets(game.m_rockets);
      fixupCamera(game);

      animateMonsters(game);
      orientPlayers(game.m_players);
      orientMonsters(game.m_monsters);
      updatePlayerAim(game.m_players);

      fireWeapons(game);

      checkMonsterHurtingPlayer(game);
      checkPickups(game);

      checkPlayerDeath(game);
      checkLevelup(game);

      updateBullets(game);
      updateRockets(game);
      updateBlasts(game);
      updateBonuses(game);
      updatePickups(game);
      updateDefenseMatrix(game);
      updateTimeCapsule(game);
      updateHealingCloud(game);
      updateTurret(game);

      updatePerks(game);
      VKillLocations locations;
      killMonsters(game, locations, true);
      generatePickups(locations, game);
      generateSplatter(locations, game);

      if( game.m_players.empty() && game.m_blasts.empty() )
      {
         if( game.m_flavour == -1 )
         {
            game.m_flavour = game.m_randomGenerator.randInt(0, 5);
         }
         if (game.m_totalKillCount > game.m_highScore)
         {
            game.m_highScore = game.m_totalKillCount;
         }
         game.m_deathTimer.updateBy(game.m_logicTimer.getDeltaMicros());
         if( game.m_deathTimer.hasElapsed() )
         {
            cleanSession(game);
            game.m_nextGameState = RainbowlandGame::GS_SessionPreparation;
         }
      }
   }

   void render_session(RainbowlandGame& game)
   {
      game.m_graphicsSystem.applyCamera(game.m_camera);
      game.m_graphicsSystem.setPerspectiveProjection();
      Vec2f atlasSize = game.m_textureCache.getTextureDimensions(game.m_atlasTexture);

      draw_splatters_to_texture(game, atlasSize);

      draw_background(game, atlasSize);

      draw_monsters(game, atlasSize);

      draw_pickups(game, atlasSize);

      draw_players(game, atlasSize);

      draw_damagers(game, atlasSize);

      draw_skills(game, atlasSize);

#if 0//GRID_DRAWING
      draw_grid_debug_info(game.m_monsterGrid, game.m_graphicsSystem);
#endif

      //****************************
      //			gui from now on
      //****************************
      game.m_graphicsSystem.clearCamera();
      game.m_graphicsSystem.setOrthographicProjection();
      if( game.m_players.empty() && game.m_blasts.empty() )
      {
         const char* flavours[] =
         {
            "HAHA YOU ARE DEAD",
               "LOLZ YOU CROAKED",
               "I THINK SOMEBODY DIED",
               "THAT WAS A GOOD DEATH",
               "MY MOTHER ONCE SCORED HIGHER",
               "YOUR LAST GAME WAS WAY BETTER"
         };
/*         const char* flavours2[] =
         {
            "What a loser",
            "So bad.",
            "Such low.",
            "He sucked most.",
            "Haha lame."
         };*/
         const char* classes[4]
         {
            "Healther", "Defenser", "Turreter", "Slower"
         };

         drawSolidQuad(game.m_graphicsSystem, {600, 400}, {}, {0.1f, 0.1f, 0.1f, 0.7f});
         auto text = "You managed to kill " + std::to_string(game.m_totalKillCount) + " thingies before being humiliated!";
         Transform t{{0, 140}, {1, 1}, 0};
         if( game.m_flavour != -1 )
         {
            drawText(game.m_graphicsSystem, game.m_defaultFont, flavours[game.m_flavour], t, {}, TJ_Center, false);
         }
         t.position.y -= 40;
         drawText(game.m_graphicsSystem, game.m_defaultFont, text, t, {}, TJ_Center, false);
         t.position.y -= 40;
         drawText(game.m_graphicsSystem, game.m_defaultFont, "Try harder next time! Highest so far: " + std::to_string(game.m_highScore), t, {}, TJ_Center, false);
         t.position.y -= 40;
         for (auto& player : game.m_deadPlayers)
         {
            auto text = classes[player.id] + std::string(" killed ") + std::to_string(player.killCount) + " monsters.";
            drawText(game.m_graphicsSystem, game.m_defaultFont, text, t, {}, TJ_Center, false);
            t.position.y -= 40;
         }
      }

      Transform tf;
      tf.scale.set(0.75f, 0.75f);
      tf.position.set(10 - (float)game.m_window->getSizeX()*0.5f, (float)game.m_window->getSizeY()*0.5f-10);

      {
         auto text = "Monster count: " + std::to_string(game.m_monsters.size());
         drawText(game.m_graphicsSystem, game.m_defaultFont, text, tf, {}, TJ_Left, false);
         tf.position.y -= 20;
         text = "Experience: " + std::to_string(game.m_experience) + " / " + std::to_string(game.m_experienceForNextLevel);
         drawText(game.m_graphicsSystem, game.m_defaultFont, text, tf, {}, TJ_Left, false);
#if 0
         tf.position.y -= 20;

         if( !game.m_players.empty() )
         {
            auto cell = calculateCellCoords(game.m_monsterGrid, game.m_players.front().aim);
            text = "Mouse cell coords: " + std::to_string(cell.x) + "," + std::to_string(cell.y);
            drawText(game.m_graphicsSystem, game.m_defaultFont, text, tf, {}, TJ_Left, false);
         }
#endif
      }
      /*{
         tf.position.y -= 20;
         auto text = std::to_string(game.m_gameplayTimer.getTimeScale());
         drawText(game.m_graphicsSystem, game.m_defaultFont, text, tf, {0, 0, 0}, TJ_Left, false);
      }*/
#ifdef _DEBUG_OFF
      {
         text = std::to_string(game.m_camera.getPosition().z);
         drawText(game.m_graphicsSystem, game.m_defaultFont, text, tf, {0, 0, 0}, TJ_Left, false);
         tf.position.y -= 20;

         text = std::to_string(game.m_difficulty);
         drawText(game.m_graphicsSystem, game.m_defaultFont, text, tf, {0, 0, 0}, TJ_Left, false);
         tf.position.y -= 20;
      }
#endif

      tf.position.y = 300;

      for( auto& b : game.m_activeBonuses )
      {
         std::string text = game.m_bonusDatabase[b.type].name;
         auto remaining = b.timer.getRemainingMicros();
         text += " " + std::to_string(static_cast<uint32_t>(microsToSeconds(remaining) + 1));
         drawText(game.m_graphicsSystem, game.m_defaultFont, text, tf, {}, TJ_Left, false);

         tf.position.y -= 20;
      }

      game.m_graphicsSystem.setPerspectiveProjection();
      game.m_graphicsSystem.applyCamera(game.m_camera);

      for( auto& player : game.m_players )
      {
         Transform t;
         t.position = player.aim;
         if( player.isAimRelative )
         {
            t.position += player.transform.position;
         }
         t.scale.set(0.5f, 0.5f);

         auto rect = game.m_rainbowlandImageDatabase[game.m_imageStartIndex_crosshair];
         float ratio = rect.halfWidth / rect.halfHeight;
         rect.center /= atlasSize;
         rect.halfWidth /= atlasSize.x;
         rect.halfHeight /= atlasSize.y;

         auto col = player.color;
         col.clip(0.9f);
         drawTexturedQuad(game.m_graphicsSystem, game.m_atlasTexture, rect, {ratio, 1}, t, col, {0});

         game.m_graphicsSystem.togglePixelShader();
         Rect image = game.m_rainbowlandImageDatabase[game.m_imageStartIndex_healthBar];
         ratio = image.halfWidth / image.halfHeight;
         image.center /= atlasSize;
         image.halfWidth /= atlasSize.x;
         image.halfHeight /= atlasSize.y;
         t.scale *= 1.5f;
         t.rotation = 0;
         Color c{0, 0, 0};
         float value = abilityPercentCharged(game, player.ability);
         bool isActive = isAbilityActive(game, player.ability);
         if( isActive || value >= 1.0f )
         {
            c = col;
            if( isActive )
            {
               value = 1 - value;
            }
         }
         drawTexturedQuad(game.m_graphicsSystem, game.m_atlasTexture, image, {ratio, 1}, t, c, value);
         game.m_graphicsSystem.togglePixelShader();

         t.scale.set(0.03f, 0.03f);
         t.position.y -= 1.0f;
         
         auto text = std::to_string(player.currentWeapon.ammo);
         if (player.currentWeapon.ammo == 0)
         {
            t.scale.set(0.02f, 0.02f);
            text = "RELOADING";
         }
         drawText(game.m_graphicsSystem, game.m_defaultFont, text, t, {0, 0, 0}, TJ_Center, false);
      }
   }
}
