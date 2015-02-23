//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Rainbowland/InputBindings.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Games/Rainbowland/Rainbowland.h>
#include <Games/Rainbowland/GameDefinitions.h>
#include <Window/Window.h>
/******* end headers *******/

namespace Core
{
#ifndef DEPLOY
   extern bool g_step;
   extern bool g_stepBack;
   extern double g_debugPauseTime;
   extern bool g_spawnEnabled;
   extern uint32_t g_mx;
   extern uint32_t g_my;
#endif
   void setupDebugInputBindings(RainbowlandGame& game)
   {
#ifndef DEPLOY
      game.m_messageHandlers.emplace_back([&](const WindowEvent& w)
      {
         if( w.m_type == WindowEventType::WE_MOUSEMOVE )
         {
            if( w.m_mouseMove.m_isRelative )
            {
               g_mx += w.m_mouseMove.m_x;
               g_my += w.m_mouseMove.m_y;
            }
            else
            {
               g_mx = w.m_mouseMove.m_x;
               g_my = w.m_mouseMove.m_y;
            }
         }
         return false;
      });

      game.m_messageHandlers.emplace_back([&](const WindowEvent& w)
      {
         if( game.m_currentGameState == RainbowlandGame::GS_Session &&
            w.m_type == WindowEventType::WE_KEYBOARDKEY &&
            w.m_keyboard.m_isDown && !w.m_keyboard.m_previouslyDown &&
            game.m_players.size() >= 1 )
         {
            switch( w.m_keyboard.m_keyCode )
            {
               case Keyboard::m_1:
                  selectWeapon(game.m_players[0], Pistol, game.m_weaponDatabase);
                  break;

               case Keyboard::m_2:
                  selectWeapon(game.m_players[0], Shotgun, game.m_weaponDatabase);
                  break;

               case Keyboard::m_3:
                  selectWeapon(game.m_players[0], Uzi, game.m_weaponDatabase);
                  break;

               case Keyboard::m_4:
                  selectWeapon(game.m_players[0], Sniper, game.m_weaponDatabase);
                  break;

               case Keyboard::m_I:
                  if( game.m_players[0].maxHealth == 100 )
                     game.m_players[0].maxHealth = game.m_players[0].health = 1000000000;
                  else
                     game.m_players[0].maxHealth = 100;
                  break;


            }
         }
         return false;
      });

      game.m_messageHandlers.emplace_back([&](const WindowEvent& w)
      {
         if( game.m_currentGameState == RainbowlandGame::GS_Session && w.m_type == WindowEventType::WE_KEYBOARDKEY )
         {
            Vec2f loc = game.m_graphicsSystem.screenToWorld({(float)g_mx, (float)g_my}, game.m_camera);
            bool single = w.m_keyboard.m_isDown && !w.m_keyboard.m_previouslyDown;
            switch( w.m_keyboard.m_keyCode )
            {
               case Keyboard::m_Space:
               {
                  if( single )
                  {
                     auto currentScale = game.m_gameplayTimer.getTimeScale();
                     game.m_gameplayTimer.setTimeScale(g_debugPauseTime);
                     g_debugPauseTime = currentScale;
                  }
               } break;

               case Keyboard::m_R:
               {
                  if( single )
                  {
                     game.m_monsters.clear();
                     game.m_monsterPool.releaseAll();
                     removeAllMonstersFromGrid(game.m_monsterGrid);
                     g_spawnEnabled = !g_spawnEnabled;
                  }
               } break;

               case Keyboard::m_E:
               {
                  if( single )
                  {
                     grantExperience(5000, game);
                  }
               } break;

               case Keyboard::m_Enter:
               {
                  if( single )
                  {
                     g_step = true;
                  }
               } break;
               case Keyboard::m_Backspace:
               {
                  if( single )
                  {
                     g_stepBack = true;
                  }
               } break;

               case Keyboard::m_L:
               {
                  if( single )
                  {
                     game.m_window->lockCursor(true);
                  }
               } break;
               case Keyboard::m_U:
               {
                  if( single )
                  {
                     game.m_window->lockCursor(false);
                  }
               } break;

               case Keyboard::m_X:
               {
                  generateMonster(game.m_monsters, loc, game);
               } break;

               case Keyboard::m_Home:
               {
                  if( single )
                  {
                     auto p = game.m_camera.getPosition();
                     p.z = -50;
                     game.m_camera.setPosition(p);
                  }
               } break;

               case Keyboard::m_PageUp:
               {
                  auto p = game.m_camera.getPosition();
                  p.z += 1;
                  game.m_camera.setPosition(p);
               } break;

               case Keyboard::m_PageDown:
               {
                  auto p = game.m_camera.getPosition();
                  p.z -= 1;
                  game.m_camera.setPosition(p);
               } break;

               case Keyboard::m_B:
               {
                  if( single )
                  {
                     generateBlast(game.m_blasts, loc, 0, 10, 1, 10, nullptr);
                  }
               } break;


               case Keyboard::m_F1:
               {
                  if( single )
                  {
                     placePickup(game, loc, IncreasedMovementSpeed, WeaponTypeCount);
                  }
               } break;

               case Keyboard::m_F2:
               {
                  if( single )
                  {
                     placePickup(game, loc, Heal, WeaponTypeCount);
                  }
               } break;

               case Keyboard::m_F3:
               {
                  if( single )
                  {
                     placePickup(game, loc, IncreasedRateOfFire, WeaponTypeCount);
                  }
               } break;

               case Keyboard::m_F4:
               {
                  if( single )
                  {
                     placePickup(game, loc, BonusTypeCount, Pistol);
                  }
               } break;

               case Keyboard::m_F5:
               {
                  if( single )
                  {
                     placePickup(game, loc, BonusTypeCount, Shotgun);
                  }
               } break;

               case Keyboard::m_F6:
               {
                  if( single )
                  {
                     placePickup(game, loc, BonusTypeCount, Uzi);
                  }
               } break;

               case Keyboard::m_F7:
               {
                  if( single )
                  {
                     placePickup(game, loc, SlowTime, WeaponTypeCount);
                  }
               } break;

               case Keyboard::m_F8:
               {
                  if( single )
                  {
                     placePickup(game, loc, BonusTypeCount, Sniper);
                  }
               } break;

               case Keyboard::m_F9:
               {
                  if( single )
                  {
                     placePickup(game, loc, BonusTypeCount, RPG);
                  }
               } break;
            }
         }
         return false;
      });
#endif
   }

   void registerKeyboardMouse(RainbowlandGame& game, uint32_t playerID)
   {
      auto playerIndex = filterFind(game.m_players, [=](const Player& p) { return p.id == playerID; });
      if( playerIndex == game.m_players.size() )
      {
         return;
      }

      game.m_players[playerIndex].isAimRelative = false;

      auto sessionHandler = [=, &game](const WindowEvent& we) -> bool
      {
         auto playerIndex = filterFind(game.m_players, [=](const Player& p) { return p.id == playerID; });
         if( playerIndex == game.m_players.size() )
         {
            return false;
         }
         auto& player = game.m_players[playerIndex];

         switch( we.m_type )
         {
            case WE_KEYBOARDKEY:
            {
               auto firstDown = we.m_keyboard.m_isDown && !we.m_keyboard.m_previouslyDown;
               auto wentUp = !we.m_keyboard.m_isDown && we.m_keyboard.m_previouslyDown;
               switch( we.m_keyboard.m_keyCode )
               {
                  case Keyboard::m_W:
                  {
                     if( firstDown )
                        player.directionActive[Up] = true;
                     if( wentUp )
                        player.directionActive[Up] = false;

                     return true;
                  } break;

                  case Keyboard::m_S:
                  {
                     if( firstDown )
                        player.directionActive[Down] = true;
                     if( wentUp )
                        player.directionActive[Down] = false;

                     return true;
                  } break;

                  case Keyboard::m_A:
                  {
                     if( firstDown )
                        player.directionActive[Left] = true;
                     if( wentUp )
                        player.directionActive[Left] = false;

                     return true;
                  } break;

                  case Keyboard::m_D:
                  {
                     if( firstDown )
                        player.directionActive[Right] = true;
                     if( wentUp )
                        player.directionActive[Right] = false;

                     return true;
                  } break;
               }
            } break;

            case WE_MOUSEBUTTON:
            {
               if( we.m_mouseButton.m_button == Mouse::m_LeftButton )
               {
                  auto wp = game.m_graphicsSystem.screenToWorld({(float)we.m_mouseButton.m_x,
                                                                (float)we.m_mouseButton.m_y},
                                                                game.m_camera);
                  if( we.m_mouseMove.m_isRelative )
                  {
                     player.aim += wp;
                  }
                  else
                  {
                     player.aim = wp;
                  }

                  if( we.m_mouseButton.m_isDown )
                  {
                     player.isShooting = true;
                  }
                  else
                  {
                     player.isShooting = false;
                  }
                  return true;
               }
               else if( we.m_mouseButton.m_button == Mouse::m_RightButton &&
                       we.m_mouseButton.m_isDown )
               {
                  activateAbility(player, game);
                  return true;
               }
            } break;

            case WE_MOUSEMOVE:
            {
               if( we.m_mouseMove.m_isRelative )
               {
                  Vec2f screen{game.m_window->getSizeX() / 2.0f, game.m_window->getSizeY() / 2.0f};
                  Vec2f mp{(float)we.m_mouseMove.m_x, (float)we.m_mouseMove.m_y};
                  auto sp = screen + mp;
                  auto wp = game.m_graphicsSystem.screenToWorld(sp, game.m_camera);
                  auto spp = game.m_graphicsSystem.screenToWorld(screen, game.m_camera);
                  player.aim += wp - spp;
               }
               else
               {
                  Vec2f sp{(float)we.m_mouseMove.m_x, (float)we.m_mouseMove.m_y};
                  auto wp = game.m_graphicsSystem.screenToWorld(sp, game.m_camera);
                  player.aim = wp;
               }
               return true;
            } break;
         }

         return false;
      };
      game.m_sessionHandlers.emplace_back(sessionHandler);



      auto sessionPerkMenuHandler = [=, &game](const WindowEvent& we) -> bool
      {
         auto playerIndex = filterFind(game.m_players, [=](const Player& p) { return p.id == playerID; });
         if( playerIndex == game.m_players.size() )
         {
            return false;
         }
         auto& player = game.m_players[playerIndex];
         if( player.chosenPerk != PerkTypeCount )
         {
            return false;
         }

         switch( we.m_type )
         {
            case WE_KEYBOARDKEY:
            {
               auto firstDown = we.m_keyboard.m_isDown && !we.m_keyboard.m_previouslyDown;
               switch( we.m_keyboard.m_keyCode )
               {
                  case Keyboard::m_A:
                  {
                     if( firstDown )
                     {
                        --player.selectedPerkIndex;
                        if( player.selectedPerkIndex == (uint32_t)-1 )
                        {
                           player.selectedPerkIndex += player.perksPerLevel;
                        }
                     }
                     return true;
                  } break;

                  case Keyboard::m_D:
                  {
                     if( firstDown )
                     {
                        player.selectedPerkIndex = (++player.selectedPerkIndex) % player.perksPerLevel;
                     }
                     return true;
                  } break;
               }
            } break;

            case WE_MOUSEBUTTON:
            {
               if( we.m_mouseButton.m_button == Mouse::m_RightButton && we.m_mouseButton.m_isDown )
               {
                  player.chosenPerk = player.selectablePerks[player.selectedPerkIndex];
                  return true;
               }
            } break;
         }
         return false;
      };
      game.m_sessionPerkMenuHandlers.emplace_back(sessionPerkMenuHandler);
   }

   void registerGamepad(RainbowlandGame& game, uint32_t playerID, uint32_t gamepadID)
   {
      auto playerIndex = filterFind(game.m_players, [=](const Player& p) { return p.id == playerID; });
      if( playerIndex == game.m_players.size() )
      {
         return;
      }

      game.m_players[playerIndex].isAimRelative = true;

      auto sessionHandler = [=, &game](const WindowEvent& we) -> bool
      {
         auto playerIndex = filterFind(game.m_players, [=](const Player& p) { return p.id == playerID; });
         if( playerIndex == game.m_players.size() )
         {
            return false;
         }
         auto& player = game.m_players[playerIndex];


         switch( we.m_type )
         {
            case WE_GAMEPADAXIS:
            {
               auto gid = we.m_gamepadAxis.m_gamepad;
               if( gid != gamepadID ) return false;

               if( we.m_gamepadAxis.m_axis == Gamepad::m_LeftStick )
               {
                  Vec2f direction{we.m_gamepadAxis.m_x, we.m_gamepadAxis.m_y};
                  float magnitude;
                  if( we.m_gamepadAxis.m_normalizedMagnitude >= 0.2f )
                  {
                     magnitude = 1;
                     player.targetDirection = direction;
                  }
                  else
                  {
                     magnitude = 0;
                     player.targetDirection = player.direction;
                  }
                  player.currentSpeed = magnitude*player.maxSpeed;
                  return true;
               }

               if( we.m_gamepadAxis.m_axis == Gamepad::m_RightStick )
               {
                  if( we.m_gamepadAxis.m_magnitude > 0 )
                  {
                     player.aimDirection.set(we.m_gamepadAxis.m_x, we.m_gamepadAxis.m_y);
                     player.aimDistance = we.m_gamepadAxis.m_normalizedMagnitude;
                  }
                  else
                  {
                     player.aimDistance = 0;
                  }
                  return true;
               }
            } break;
            case WE_GAMEPADBUTTON:
            {
               if( we.m_gamepadButton.m_button == Gamepad::m_RightTrigger )
               {
                  if( we.m_gamepadButton.m_isDown )
                  {
                     player.isShooting = true;
                  }
                  else
                  {
                     player.isShooting = false;
                  }
                  return true;
               }
               if( we.m_gamepadButton.m_button == Gamepad::m_LeftTrigger )
               {
                  if( we.m_gamepadButton.m_isDown )
                  {
                     activateAbility(player, game);
                  }
                  return true;
               }

            } break;
         }
         return false;
      };
      game.m_sessionHandlers.emplace_back(sessionHandler);




      auto sessionPerkMenuHandler = [=, &game](const WindowEvent& we) -> bool
      {
         auto playerIndex = filterFind(game.m_players, [=](const Player& p) { return p.id == playerID; });
         if( playerIndex == game.m_players.size() )
         {
            return false;
         }
         auto& player = game.m_players[playerIndex];
         if( player.chosenPerk != PerkTypeCount )
         {
            return false;
         }

         switch( we.m_type )
         {
            case WE_GAMEPADBUTTON:
            {
               auto gid = we.m_gamepadButton.m_gamepad;
               if( gid != gamepadID ) return false;

               if( we.m_gamepadButton.m_button == Gamepad::m_DPadRight && we.m_gamepadButton.m_isDown )
               {

                  player.selectedPerkIndex = (++player.selectedPerkIndex) % player.perksPerLevel;
                  return true;
               }
               if( we.m_gamepadButton.m_button == Gamepad::m_DPadLeft && we.m_gamepadButton.m_isDown )
               {
                  --player.selectedPerkIndex;
                  if( player.selectedPerkIndex == (uint32_t)-1 )
                  {
                     player.selectedPerkIndex += player.perksPerLevel;
                  }
                  return true;
               }
               if( we.m_gamepadButton.m_button == Gamepad::m_LeftTrigger && we.m_gamepadButton.m_isDown )
               {
                  player.chosenPerk = player.selectablePerks[player.selectedPerkIndex];
                  return true;
               }
            } break;
         }
         return false;
      };
      game.m_sessionPerkMenuHandlers.emplace_back(sessionPerkMenuHandler);
   }
}
