//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Rainbowland/Session.h>
/******* C++ headers *******/
/******* extra headers *******/
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
      for( auto& obj : game.m_monsters )
      {
         obj.objectTimer.updateBy(game.m_gameplayTimer.getDeltaMicros());
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
      updateBlink(game);
      updateTurret(game);

      updatePerks(game);
      VKillLocations locations;
      killMonsters(game, locations, true);
      generatePickups(locations, game);
      generateSplatter(locations, game);

      if( game.m_players.size() == 0 )
      {
         if( game.m_flavour == -1 )
         {
            game.m_flavour = game.m_randomGenerator.randInt(0, 6);
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
      Vec2 atlasSize = game.m_textureCache.getTextureDimensions(game.m_atlasTexture);


      {
         //draw splatters to texture render target
         game.m_graphicsSystem.v3_setTextureAsRenderTarget();
         auto indices = game.m_graphicsSystem.v3_makeSolidQuadIndices();
         game.m_graphicsSystem.setTransparencyMode(true);
         game.m_graphicsSystem.v3_setTexture(game.m_atlasTexture);
         for( auto splatter : game.m_splatters )
         {
            auto sdb = game.m_rainbowlandImageDatabase[splatter.splatterImage];
            float ratio = sdb.halfWidth / sdb.halfHeight;
            sdb.halfWidth /= atlasSize.x;
            sdb.halfHeight /= atlasSize.y;
            sdb.center /= atlasSize;
            auto vertices = game.m_graphicsSystem.v3_makeQuadVertices({}, {ratio, 1});
            vertices[0].setTextureCoords(sdb.left(), sdb.bottom());
            vertices[1].setTextureCoords(sdb.right(), sdb.bottom());
            vertices[2].setTextureCoords(sdb.left(), sdb.top());
            vertices[3].setTextureCoords(sdb.right(), sdb.top());

            game.m_graphicsSystem.v3_setVertices(vertices);
            game.m_graphicsSystem.v3_setIndices(indices);
            game.m_graphicsSystem.v3_setInstanceData({splatter.transform}, {splatter.color});
            game.m_graphicsSystem.v3_setTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            game.m_graphicsSystem.v3_draw(indices.size(), 1);
         }

         game.m_splatters.clear();
         game.m_graphicsSystem.v3_clearTextureAsRenderTarget();
      }
      game.m_graphicsSystem.setPerspectiveProjection();
      game.m_graphicsSystem.applyCamera(game.m_camera);
      {
         auto vertices = game.m_graphicsSystem.v3_makeQuadVertices({}, game.m_playingField.halfSize());
         vertices[0].setTextureCoords(0, 0);
         vertices[1].setTextureCoords(1, 0);
         vertices[2].setTextureCoords(0, 1);
         vertices[3].setTextureCoords(1, 1);
         auto indices = game.m_graphicsSystem.v3_makeSolidQuadIndices();

         game.m_graphicsSystem.v3_setVertices(vertices);
         game.m_graphicsSystem.v3_setIndices(indices);
         game.m_graphicsSystem.v3_setInstanceData({{}}, {{0.6f, 0.6f, 0.6f}});
         game.m_graphicsSystem.v3_setTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

         game.m_graphicsSystem.setTransparencyMode(false);
         game.m_graphicsSystem.v3_setTexture(game.m_backgroundTexture);
         game.m_graphicsSystem.v3_draw(indices.size(), 1);

         //draw the texture render target over the screen
         game.m_graphicsSystem.setTransparencyMode(true);
         game.m_graphicsSystem.v3_setTextureFromRenderTarget();
         game.m_graphicsSystem.v3_draw(indices.size(), 1);
      }

      if( game.m_monsters.size() > 0 )
      {
         //monsters
         for( auto& obj : game.m_monsters )
         {
            Rect r;
            switch( obj.type )
            {
               case Flower:
               {
                  r = game.m_rainbowlandImageDatabase[game.m_imageStartIndex_flower];
               } break;

               case Butterfly:
               {
                  r = game.m_rainbowlandImageDatabase[game.m_imageStartIndex_butterfly];
               } break;

               case Ladybug:
               {
                  r = game.m_rainbowlandImageDatabase[game.m_imageStartIndex_ladybug];
               } break;
            }

            float ratio = r.halfWidth / r.halfHeight;
            r.center /= atlasSize;
            r.halfWidth /= atlasSize.x;
            r.halfHeight /= atlasSize.y;

            drawTexturedQuad(game.m_graphicsSystem, game.m_atlasTexture, r, {ratio, 1}, obj.transform, obj.color);
#if 0
            switch( obj.brain.state )
            {
               case BS_Attack:
               {
                  Transform textTf{obj.transform.position, {0.02f, 0.02f}, 0};
                  drawText(game.m_graphicsSystem, game.m_defaultFont, "Attack", textTf, obj.color, TJ_Center, false);
               } break;
               case BS_Wander:
               {
                  Transform textTf{obj.transform.position, {0.02f, 0.02f}, 0};
                  drawText(game.m_graphicsSystem, game.m_defaultFont, "Wander", textTf, obj.color, TJ_Center, false);
               } break;
               case BS_Flank:
               {
                  Transform textTf{obj.transform.position, {0.02f, 0.02f}, 0};
                  drawText(game.m_graphicsSystem, game.m_defaultFont, "Flank", textTf, obj.color, TJ_Center, false);
               } break;
            }*/
            //drawLine(game.m_graphicsSystem, obj.transform.position, obj.brain.targetLocation, {}, obj.color);
#endif


#ifdef _DEBUG_OFF
            auto t = obj.transform;
            t.position += obj.collisionData_attack.center;
            drawHollowCircle(game.m_graphicsSystem, obj.collisionData_attack.radius, 18, t, {});
            t = obj.transform;
            t.position += obj.collisionData_hitbox.center;
            drawHollowCircle(game.m_graphicsSystem, obj.collisionData_hitbox.radius, 18, t, {});
#endif
         }
      }

      {
         // pickups
         for( auto& pickup : game.m_pickups )
         {
            auto imageRect = game.m_rainbowlandImageDatabase[pickup.iconIndex];
            auto circleRect = game.m_rainbowlandImageDatabase[game.m_imageStartIndex_bonuses + 6];

            imageRect.center /= atlasSize;
            imageRect.halfWidth /= atlasSize.x;
            imageRect.halfHeight /= atlasSize.y;
            circleRect.center /= atlasSize;
            circleRect.halfWidth /= atlasSize.x;
            circleRect.halfHeight /= atlasSize.y;

            Vec2 scale{1, 1};
            if( pickup.bonus != BonusTypeCount )
               drawTexturedQuad(game.m_graphicsSystem, game.m_atlasTexture, circleRect, {1.5f, 1.5f}, pickup.transform, pickup.color);
            else
               scale.set(2.0f, 2.0f);
            drawTexturedQuad(game.m_graphicsSystem, game.m_atlasTexture, imageRect, scale, pickup.transform, pickup.color);
         }
      }

      if( game.m_players.size() > 0 )
      {
         //players
         for( auto& obj : game.m_players )
         {
            Rect r = game.m_rainbowlandImageDatabase[game.m_imageStartIndex_players + obj.id];
            float ratio = r.halfWidth / r.halfHeight;
            r.center /= atlasSize;
            r.halfWidth /= atlasSize.x;
            r.halfHeight /= atlasSize.y;

            drawTexturedQuad(game.m_graphicsSystem, game.m_atlasTexture, r, {ratio, 1}, obj.transform, obj.color);

            Transform tHP{obj.transform.position+Vec2{0,1}, {0.03f, 0.03f}, 0};
            std::string hpExp = std::to_string(obj.health);
            drawText(game.m_graphicsSystem, game.m_defaultFont, hpExp, tHP, obj.color, TJ_Center, false);

            auto time = obj.abilityTimeLeft(game);
            auto text = std::to_string(time);
            tHP.position = obj.transform.position + Vec2{0, -1};
            drawText(game.m_graphicsSystem, game.m_defaultFont, text, tHP, obj.color, TJ_Center, false);


            Transform tDir{obj.transform.position + obj.direction * 2, {0.3f, 0.3f}, 0};
            drawSolidCircle(game.m_graphicsSystem, 1, 8, tDir, obj.color);
         }
#ifdef _DEBUG_OFF
         std::vector<Transform> tfs;
         std::vector<Color> fills;
         tfs.reserve(game.m_players.size());
         fills.reserve(game.m_players.size());
         for( auto& obj : game.m_players )
         {
            tfs.emplace_back(obj.transform);
            auto col = obj.color;
            fills.emplace_back(col);
         }

         auto inds = game.m_graphicsSystem.v3_makeHollowCircleIndices(18);
         auto verts = game.m_graphicsSystem.v3_makeCircleVertices(game.m_players.front().collisionData.center,
                                                                  game.m_players.front().collisionData.radius, 18);
         game.m_graphicsSystem.v3_setVertices(verts);
         game.m_graphicsSystem.v3_setIndices(inds);
         game.m_graphicsSystem.v3_setInstanceData(tfs, fills);
         game.m_graphicsSystem.v3_setTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
         game.m_graphicsSystem.v3_draw(inds.size(), game.m_players.size());
#endif
      }

      for( auto& obj : game.m_bullets )
      {
         drawLine(game.m_graphicsSystem, obj.trail, {1, 1, 1, 0}, obj.position, {1, 1, 1, 1}, {}, {});
      }
      for( auto& obj : game.m_rockets )
      {
         Rect rect = game.m_rainbowlandImageDatabase[game.m_imageStartIndex_rocket];
         float ratio = rect.halfWidth / rect.halfHeight;
         rect.center /= atlasSize;
         rect.halfWidth /= atlasSize.x;
         rect.halfHeight /= atlasSize.y;

         Transform t{obj.body.center, {1, 1}, std::atan2(obj.direction.y, obj.direction.x)};
         t.rotation = Deg2Rad(Rad2Deg(t.rotation) - 90);
         drawTexturedQuad(game.m_graphicsSystem, game.m_atlasTexture, rect, {ratio,1}, t, {});
      }
      for( auto& obj : game.m_blasts )
      {
         Transform t{obj.area.center, {1, 1}, obj.rotation};
         Color c{1, 1, 1};
         Rect img = game.m_rainbowlandImageDatabase[game.m_imageStartIndex_blast];
         img.center /= atlasSize;
         img.halfWidth /= atlasSize.x;
         img.halfHeight /= atlasSize.y;

         Vec2 hs{obj.area.radius*1.1f, obj.area.radius*1.1f};

         float percent = obj.area.radius / obj.maxRadius;
         if( percent > 0.8f )
         {
            percent = 1 - percent;
            c.a *= (percent / (0.2f));
         }

         drawTexturedQuad(game.m_graphicsSystem, game.m_atlasTexture, img, hs, t, c);
         //drawHollowCircle(m_graphicsSystem, obj.area.radius, 36, t, c);
      }

      if( game.m_defenseMatrix.active )
      {
         Transform t{game.m_defenseMatrix.area.center, {1, 1}, 0};
         drawSolidCircle(game.m_graphicsSystem, game.m_defenseMatrix.area.radius, 36, t, {0.25f, 0.42f, 0.76f, 0.2f});
         auto displayTime = defenseMatrixTimeLeft(game);
         {
            auto text = std::to_string(displayTime);
            t.scale.set(0.03f, 0.03f);
            drawText(game.m_graphicsSystem, game.m_defaultFont, text, t, {0, 0, 0}, TJ_Left, false);
         }
      }
      if( game.m_timeCapsule.active )
      {
         Transform t{game.m_timeCapsule.area.center, {1, 1}, 0};
         auto vertices = game.m_graphicsSystem.v3_makeCircleVertices({}, game.m_timeCapsule.area.radius, 36);
         auto indices = game.m_graphicsSystem.v3_makeSolidCircleIndices(36);
         for( auto& v : vertices )
         {
            v.setDiffuse(0.75f, 0.42f, 0.2f, 0.0f);
         }
         vertices[0].setDiffuse(0.75f, 0.42f, 0.2f, 0.8f);

         game.m_graphicsSystem.v3_setVertices(vertices);
         game.m_graphicsSystem.v3_setIndices(indices);
         game.m_graphicsSystem.v3_setInstanceData({t}, {{}});
         game.m_graphicsSystem.v3_setTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
         game.m_graphicsSystem.v3_draw(indices.size(), 1);

         auto displayTime = timeCapsuleTimeLeft(game);
         {
            auto text = std::to_string(displayTime);
            t.scale.set(0.03f, 0.03f);
            drawText(game.m_graphicsSystem, game.m_defaultFont, text, t, {0, 0, 0}, TJ_Left, false);
         }
      }
      if( game.m_turret.active )
      {
         Transform t{game.m_turret.area.center, {1, 1}, 0};
         Rect rect = game.m_rainbowlandImageDatabase[game.m_imageStartIndex_turret];
         float ratio = rect.halfWidth / rect.halfHeight;
         rect.center /= atlasSize;
         rect.halfHeight /= atlasSize.y;
         rect.halfWidth /= atlasSize.x;
         drawTexturedQuad(game.m_graphicsSystem, game.m_atlasTexture, rect, {ratio, 1}, t, {});
         auto displayTime = turretTimeLeft(game);
         {
            auto text = std::to_string(displayTime);
            t.scale.set(0.03f, 0.03f);
            t.position.y -= 1;
            drawText(game.m_graphicsSystem, game.m_defaultFont, text, t, {0, 0, 0}, TJ_Left, false);
         }
      }
      if( game.m_blink.active )
      {
         Transform t{game.m_blink.area.center, {1, 1}, 0};
         drawHollowCircle(game.m_graphicsSystem, game.m_blink.area.radius, 18, t, {0.7f, 0.7f, 0});
         t.position = game.m_blink.target;
         drawHollowCircle(game.m_graphicsSystem, game.m_blink.area.radius, 18, t, {0.7f, 0.7f, 0});
      }

      //****************************
      //			gui from now on
      //****************************
      game.m_graphicsSystem.clearCamera();
      game.m_graphicsSystem.setOrthographicProjection();
      if( game.m_players.size() == 0 )
      {
         const char* flavours[] =
         {
            "HAHA YOU ARE DEAD",
               "LOLZ YOU CROAKED",
               "YO YO YO SOMEBODY DIED",
               "DUDE YOU KINDA SUCK",
               "HAD FUN DYING LOL",
               "MY MOM COULD BEAT THAT SCORE",
               "YOUR LAST GAME WAS WAY BETTER",
               "MARAC JE SUPAK!"
         };
         const char* flavours2[] =
         {
            "What a loser",
            "So bad.",
            "Such low.",
            "He sucked most.",
            "Haha lame."
         };
         const char* classes[4]
         {
            "Blinker", "Defenser", "Turreter", "Slower"
         };

         drawSolidQuad(game.m_graphicsSystem, {600, 400}, {}, {0.1f, 0.1f, 0.1f});
         auto text = "You managed to kill " + std::to_string(game.m_totalKillCount) + " thingies before being humiliated!";
         Transform t{{0, 140}, {1, 1}, 0};
         drawText(game.m_graphicsSystem, game.m_defaultFont, flavours[game.m_flavour], t, {}, TJ_Center, false);
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

      /*
      if( game.m_blinkPlaying )
      {
         timeLeft = game.m_blink.updateDelay - game.m_blink.timer.getCurrentMicros();
         displayTime = static_cast<uint32_t>(Time::microsToSeconds(timeLeft) + 1);
         if( timeLeft < 0 ) displayTime = 0;
         {
            auto text = "Blink: " + std::to_string(displayTime);
            drawText(game.m_graphicsSystem, game.m_defaultFont, text, tf, {0, 0, 0}, TJ_Left, false);
         }
         tf.position.y -= 20;
      }
      */
      for( auto& b : game.m_activeBonuses )
      {
         std::string text = game.m_bonusDatabase[b.type].name;
         auto remaining = b.timer.getRemainingMicros();
         text += " " + std::to_string(static_cast<uint32_t>(nMicros::toSeconds(remaining) + 1));
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
         if( col.r < 0.9f ) col.r = 0;
         if( col.g < 0.9f ) col.g = 0;
         if( col.b < 0.9f ) col.b = 0;
         drawTexturedQuad(game.m_graphicsSystem, game.m_atlasTexture, rect, {ratio, 1}, t, col);

         t.scale.set(0.03f, 0.03f);
         t.position.y -= 1.0f;
         
         auto text = std::to_string(player.currentWeapon.ammo);
         if (player.currentWeapon.ammo == 0)
         {
            t.scale.set(0.02f, 0.02f);
            text = "RELOADING";
         }
         drawText(game.m_graphicsSystem, game.m_defaultFont, text, t, col, TJ_Center, false);
      }
   }
}
