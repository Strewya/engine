//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Rainbowland/SessionPreparation.h>
/******* C++ headers *******/
#include <algorithm>
/******* extra headers *******/
#include <Games/Rainbowland/InputBindings.h>
#include <Games/Rainbowland/Rainbowland.h>
#include <Window/WindowEvent.h>
/******* end headers *******/

namespace Core
{
   void initSession(RainbowlandGame& game);

   void logic_sessionPreparation(RainbowlandGame& game)
   {
      auto evs = game.m_inputSystem.getEvents();
      for( auto& e : evs )
      {
         for( auto& f : game.m_sessionPreparationHandlers )
         {
            if( f(e) )
            {
               continue;
            }
         }
         for( auto& f : game.m_messageHandlers )
         {
            if( f(e) )
            {
               continue;
            }
         }
      }

      using std::placeholders::_1;


      Transform tMainPanel;
      Vec2 hMainPanel;
      Color cMainPanel;

      tMainPanel.position.set(0, 0);
      hMainPanel.set(500, 400);
      cMainPanel.set(0.1f, 0.1f, 0.1f);
      game.m_renderQueue.enqueueRenderCommand(
         std::bind(drawSolidQuad, _1, hMainPanel, tMainPanel, cMainPanel));

      Transform tNotPlayingPanel;
      Vec2 hNotPlayingPanel;
      Color cNotPlayingPanel;

      hNotPlayingPanel.set(hMainPanel.x * 0.2f, hMainPanel.y - 20);
      tNotPlayingPanel.position.set(-hMainPanel.x + 20, -hMainPanel.y + 20);
      tNotPlayingPanel.position += hNotPlayingPanel;
      tNotPlayingPanel.position += tMainPanel.position;
      cNotPlayingPanel.set(0.69f, 0.27f, 0.27f);

      game.m_renderQueue.enqueueRenderCommand(
         std::bind(drawSolidQuad, _1, hNotPlayingPanel, tNotPlayingPanel, cNotPlayingPanel));

      Transform tBox;
      Vec2 hBox;
      Color cBox;

      const char* controllers[5]
      {
         "Key+Mouse", "Gamepad 1", "Gamepad 2", "Gamepad 3", "Gamepad 4"
      };

      float cellHeight = hNotPlayingPanel.y / 5;
      for( uint32_t i = 0; i < 5; ++i )
      {
         tBox.position.set(0, hNotPlayingPanel.y - cellHeight - cellHeight * 2 * i);
         tBox.position += tNotPlayingPanel.position;
         hBox.set(hNotPlayingPanel.x *0.9f, cellHeight * 0.9f);
         cBox.set(0.3f, 0.3f, 0.3f);

         game.m_renderQueue.enqueueRenderCommand(
            std::bind(drawSolidQuad, _1, hBox, tBox, cBox));

         Transform tLabel;
         tLabel.position += tBox.position;
         tLabel.scale *= 0.8f;

         bool controllerTaken = game.m_preparationData.controllers[i] != -1;
         if( !controllerTaken )
         {
            game.m_renderQueue.enqueueRenderCommand(
               std::bind(drawText, _1, game.m_defaultFont, controllers[i], tLabel, Color{}, TJ_Center, false));
         }
         tLabel.position.set(0, hBox.y - 15);
         tLabel.position += tBox.position;
         tLabel.scale *= 0.5f;

         if( controllerTaken )
         {
            std::string text = "BACK";
            if( i == 0 ) text = "RMB";
            game.m_renderQueue.enqueueRenderCommand(
               std::bind(drawText, _1, game.m_defaultFont, text, tLabel, Color{}, TJ_Center, false));
         }
      }

      Transform tPlayingPanel;
      Vec2 hPlayingPanel;
      Color cPlayingPanel;

      hPlayingPanel.set(hMainPanel.x * 0.7f, hMainPanel.y * 0.8f);
      tPlayingPanel.position.set(hMainPanel.x - 20 - hMainPanel.x * 0.7f,
                                 hMainPanel.y - 20 - hMainPanel.y*0.8f);
      tPlayingPanel.position += tMainPanel.position;
      cPlayingPanel.set(0.15f, 0.56f, 0.15f);

      game.m_renderQueue.enqueueRenderCommand(
         std::bind(drawSolidQuad, _1, hPlayingPanel, tPlayingPanel, cPlayingPanel));

      const char* classes[4]
      {
         "Blinker", "Defenser", "Turreter", "Slower"
      };
      const char* keys[4]
      {
         "W / Y", "D / B", "A / X", "S / A"
      };

      Vec2 cellSize = hPlayingPanel / 2;
      for( uint32_t i = 0; i < 2; ++i )
      {
         for( uint32_t j = 0; j < 2; ++j )
         {
            auto index = j * 2 + i;

            tBox.position.set(-hPlayingPanel.x + cellSize.x + cellSize.x * 2 * i,
                              hPlayingPanel.y - cellSize.y - cellSize.y * 2 * j);
            tBox.position += tPlayingPanel.position;
            hBox = cellSize*0.7f;

            game.m_renderQueue.enqueueRenderCommand(
               std::bind(drawSolidQuad, _1, hBox, tBox, cBox));

            Transform tLabel;
            tLabel.position += tBox.position;

            game.m_renderQueue.enqueueRenderCommand(
               std::bind(drawText, _1, game.m_defaultFont, classes[index], tLabel, Color{}, TJ_Center, false));

            tLabel.scale *= 0.4f;
            tLabel.position.set(0, -hBox.y + 15);
            tLabel.position += tBox.position;

            game.m_renderQueue.enqueueRenderCommand(
               std::bind(drawText, _1, game.m_defaultFont, "LT / RMB for ability", tLabel, Color{}, TJ_Center, false));

            tLabel.position.set(0, hBox.y - 15);
            tLabel.position += tBox.position;

            std::string text = keys[index];
            if( game.m_preparationData.classes[index] != -1 )
            {
               text = controllers[game.m_preparationData.classes[index]];
               tLabel.scale *= 1.5;
            }

            game.m_renderQueue.enqueueRenderCommand(
               std::bind(drawText, _1, game.m_defaultFont, text, tLabel, Color{}, TJ_Center, false));
         }
      }

      Transform tStart;
      Vec2 hStart;
      Color cStart;

      hStart.set(100, 20);
      tStart.position.set(hMainPanel.x - hStart.x - 10, -hMainPanel.y + hStart.y+10);
      tStart.position += tMainPanel.position;
      cStart.set(0, 0, 1);

      game.m_renderQueue.enqueueRenderCommand(
         std::bind(drawSolidQuad, _1, hStart, tStart, cStart));

      Transform tLabel;
      tLabel.position += tStart.position;

      game.m_renderQueue.enqueueRenderCommand(
         std::bind(drawText, _1, game.m_defaultFont, "Start/LMB", tLabel, Color{}, TJ_Center, false));

      if( game.m_preparationData.start )
      {
         uint32_t count = 0;
         for( auto i = 0; i < 4; ++i )
         {
            if( game.m_preparationData.classes[i] != -1 )
            {
               ++count;
            }
         }
         if( count > 0 )
         {
            game.m_playerCount = count;
            initSession(game);
            game.m_nextGameState = RainbowlandGame::GS_Session;
         }
         game.m_preparationData.start = false;
      }
   }

   void render_sessionPreparation(RainbowlandGame& game)
   {
      game.m_graphicsSystem.applyCamera(game.m_camera);
      game.m_graphicsSystem.setPerspectiveProjection();

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

      game.m_graphicsSystem.clearCamera();
      game.m_graphicsSystem.setOrthographicProjection();
      game.m_graphicsSystem.setTransparencyMode(true);

      game.m_renderQueue.runAndFlushRenderCommands();
   }


   void initSession(RainbowlandGame& game)
   {
      game.m_graphicsSystem.createTextureRenderTarget(1200, 900);

      game.m_deathTimer.reset();
      game.m_gameplayTimer.reset();
      game.m_currentTimeScale = Time::NORMAL_TIME;
      game.m_gameplayTimer.setTimeScale(game.m_currentTimeScale);
      game.m_difficulty = 1;
      game.m_killCounter = 0;
      game.m_totalKillCount = 0;
      game.m_difficultyTimer.reset();
      game.m_flavour = -1;

      game.m_defenseMatrix.active = false;
      game.m_defenseMatrix.area.set(0, 0, 4);
      game.m_defenseMatrix.updateDelay = 0;
      game.m_defenseMatrix.timer.reset();
      game.m_defenseMatrix.durationSeconds = 8;
      game.m_defenseMatrix.cooldownSeconds = 20;

      game.m_timeCapsule.active = false;
      game.m_timeCapsule.area.set(0, 0, 10);
      game.m_timeCapsule.updateDelay = 0;
      game.m_timeCapsule.timer.reset();
      game.m_timeCapsule.durationSeconds = 8;
      game.m_timeCapsule.cooldownSeconds = 20;
      game.m_timeCapsule.maxHealPeriod = 40;

      game.m_blink.active = false;
      game.m_blink.area.set(0, 0, 6);
      game.m_blink.target.set(0, 0);
      game.m_blink.updateDelay = 0;
      game.m_blink.timer.reset();
      game.m_blink.durationSeconds = 0.1f;
      game.m_blink.cooldownSeconds = 20;

      game.m_turret.active = false;
      game.m_turret.area.set(0, 0, 1);
      game.m_turret.updateDelay = 0;
      game.m_turret.timer.reset();
      game.m_turret.durationSeconds = 8;
      game.m_turret.cooldownSeconds = 20;
      game.m_turret.killCount = 0;

      game.m_experience = 0;
      game.m_experienceIncrement = 5000*game.m_playerCount;
      game.m_experienceForNextLevel = 5000;
      game.m_level = 1;

      for( auto& spawner : game.m_monsterSpawners )
      {
         spawner.objectTimer.reset();
         spawner.timer.reset();
         spawner.spawnCooldown = Time::secondsToMicros(game.m_randomGenerator.randFloat() * 5 + 1)/game.m_playerCount;
      }

      game.m_players.reserve(game.m_playerCount);
      for( auto soldier = 0; soldier < 4; ++soldier )
      {
         auto controller = game.m_preparationData.classes[soldier];
         if( controller != -1 )
         {
            game.m_players.emplace_back(Player{});
            initPlayer(game.m_players.back(), soldier, game);
            if( controller == 0 )
            {
               //keyboard
               registerKeyboardMouse(game, soldier);
            }
            else
            {
               //gamepad
               registerGamepad(game, soldier, controller-1);
            }
         }
      }
   }

   void initPreparationHandlers(RainbowlandGame& game)
   {
      auto sessionPreparationHandler = [=, &game](const WindowEvent& we) -> bool
      {
         auto& prep = game.m_preparationData;
         uint32_t con = -1;
         uint32_t cls = -1;

         switch( we.m_type )
         {
            case WE_KEYBOARDKEY:
            {
               con = 0;
               switch( we.m_keyboard.m_keyCode )
               {
                  case Keyboard::m_Escape:
                  {
                     if (we.m_keyboard.m_isDown && !we.m_keyboard.m_previouslyDown)
                     {
                        game.m_isRunning = false;
                        return true;
                     }
                  } break;

                  case Keyboard::m_W:
                  {
                     cls = 0;
                  } break;

                  case Keyboard::m_S:
                  {
                     cls = 3;
                  } break;

                  case Keyboard::m_A:
                  {
                     cls = 2;
                  } break;

                  case Keyboard::m_D:
                  {
                     cls = 1;
                  } break;
               }
            } break;

            case WE_MOUSEBUTTON:
            {
               con = 0;
               if( we.m_mouseButton.m_button == Mouse::m_LeftButton && we.m_mouseButton.m_isDown )
               {
                  game.m_preparationData.start = true;
                  return true;
               }

               if(we.m_mouseButton.m_button == Mouse::m_RightButton && we.m_mouseButton.m_isDown)
               {
                  auto i = game.m_preparationData.controllers[con];
                  game.m_preparationData.classes[i] = -1;
                  game.m_preparationData.controllers[con] = -1;
                  return true;
               }
            } break;

            case WE_GAMEPADBUTTON:
            {
               con = we.m_gamepadButton.m_gamepad + 1;
               switch( we.m_gamepadButton.m_button )
               {
                  case Gamepad::m_Back:
                  {
                     auto i = game.m_preparationData.controllers[con];
                     game.m_preparationData.classes[i] = -1;
                     game.m_preparationData.controllers[con] = -1;
                     return true;
                  } break;

                  case Gamepad::m_Start:
                  {
                     game.m_preparationData.start = true;
                     return true;
                  } break;

                  case Gamepad::m_DPadUp:
                  {
                     cls = 0;
                  } break;

                  case Gamepad::m_DPadDown:
                  {
                     cls = 3;
                  } break;

                  case Gamepad::m_DPadLeft:
                  {
                     cls = 2;
                  } break;

                  case Gamepad::m_DPadRight:
                  {
                     cls = 1;
                  } break;
               }
            } break;
         }

         if( con != -1 && cls != -1 )
         {
            if( prep.classes[cls] == -1 )
            {
               prep.classes[cls] = con;
               if( prep.controllers[con] != -1 )
               {
                  prep.classes[prep.controllers[con]] = -1;
               }
               prep.controllers[con] = cls;
            }
            return true;
         }

         return false;
      };
      game.m_sessionPreparationHandlers.emplace_back(sessionPreparationHandler);
   }
}
