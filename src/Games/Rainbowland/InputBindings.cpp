//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Rainbowland/InputBindings.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Games/Rainbowland/Rainbowland.h>
#include <Games/Rainbowland/GameObjects.h>
#include <Window/Window.h>
/******* end headers *******/

namespace Core
{
#ifndef DEPLOY
	extern bool step;
	extern bool stepBack;
	extern double debugPauseTime;
	extern bool g_spawnEnabled;
	extern uint32_t mx;
	extern uint32_t my;
#endif
	void setupInputBindings(RainbowlandGame& game)
	{
		game.m_messageHandlers.emplace_back([&](const WindowEvent& w)
		{
			if( w.m_type == WindowEventType::WE_KEYBOARDKEY && w.m_keyboard.m_keyCode == Keyboard::m_Escape )
			{
				game.m_isRunning = false;
				return true;
			}
			return false;
		});

		game.m_messageHandlers.emplace_back([&](const WindowEvent& w)
		{
			if( w.m_type == WindowEventType::WE_MOUSEMOVE && game.m_players.size() >= 1 )
			{
				if( w.m_mouseMove.m_isRelative )
				{
					Vec2 screen{game.m_window->getSizeX() / 2.0f, game.m_window->getSizeY() / 2.0f};
					Vec2 mp{(float)w.m_mouseMove.m_x, (float)w.m_mouseMove.m_y};
					auto sp = screen + mp;
					auto wp = game.m_graphicsSystem.screenToWorld(sp, game.m_camera);
					auto spp = game.m_graphicsSystem.screenToWorld(screen, game.m_camera);
					game.m_players[0].aim += wp - spp;
				}
				else
				{
					Vec2 sp{(float)w.m_mouseMove.m_x, (float)w.m_mouseMove.m_y};
					auto wp = game.m_graphicsSystem.screenToWorld(sp, game.m_camera);
					game.m_players[0].aim = wp;
				}
#ifndef DEPLOY
				if( w.m_mouseMove.m_isRelative )
				{
					mx += w.m_mouseMove.m_x;
					my += w.m_mouseMove.m_y;
				}
				else
				{
					mx = w.m_mouseMove.m_x;
					my = w.m_mouseMove.m_y;
				}
#endif
				return true;
			}
			return false;
		});

		game.m_messageHandlers.emplace_back([&](const WindowEvent& w)
		{
			if( !game.perkMode && w.m_type == WindowEventType::WE_MOUSEBUTTON )
			{
				if( w.m_mouseButton.m_button == Mouse::m_LeftButton && game.m_players.size() >= 1 )
				{
					auto wp = game.m_graphicsSystem.screenToWorld({(float)w.m_mouseButton.m_x, (float)w.m_mouseButton.m_y}, game.m_camera);
					if( w.m_mouseMove.m_isRelative )
					{
						game.m_players[0].aim += wp;
					}
					else
					{
						game.m_players[0].aim = wp;
					}

					if( w.m_mouseButton.m_isDown )
					{
						game.m_players[0].isShooting = true;
					}
					else
					{
						game.m_players[0].isShooting = false;
					}
					return true;
				}
				else if( w.m_mouseButton.m_button == Mouse::m_RightButton && w.m_mouseButton.m_isDown && game.m_players.size() >= 1 )
				{
					return enterPerkMode(game);
				}
			}
			return false;
		});
#ifndef DEPLOY
		game.m_messageHandlers.emplace_back([&](const WindowEvent& w)
		{
			if( !game.perkMode && w.m_type == WindowEventType::WE_KEYBOARDKEY && 
			   w.m_keyboard.m_isDown && !w.m_keyboard.m_previouslyDown && game.m_players.size() >= 1 )
			{
				switch( w.m_keyboard.m_keyCode )
				{
					case Keyboard::m_1:
						selectWeapon(game.m_players[0], Pistol, game.m_weaponDatabase);
						return true;

					case Keyboard::m_2:
						selectWeapon(game.m_players[0], Shotgun, game.m_weaponDatabase);
						return true;

					case Keyboard::m_3:
						selectWeapon(game.m_players[0], Uzi, game.m_weaponDatabase);
						return true;

					case Keyboard::m_4:
						selectWeapon(game.m_players[0], Sniper, game.m_weaponDatabase);
						return true;

					case Keyboard::m_I:
						if( game.m_players[0].maxHealth == 100 )
							game.m_players[0].maxHealth = game.m_players[0].health = 1000000000;
						else
							game.m_players[0].maxHealth = 100;
						return true;

					case Keyboard::m_R:
						game.m_monsters.clear();
						g_spawnEnabled = !g_spawnEnabled;
						return true;
				}
			}
			return false;
		});

		game.m_messageHandlers.emplace_back([&](const WindowEvent& w)
		{
			if( !game.perkMode && w.m_type == WindowEventType::WE_KEYBOARDKEY && w.m_keyboard.m_isDown && !w.m_keyboard.m_previouslyDown )
			{
				Vec2 loc = game.m_graphicsSystem.screenToWorld({(float)mx, (float)my}, game.m_camera);
				switch( w.m_keyboard.m_keyCode )
				{
					case Keyboard::m_Space:
					{
						auto currentScale = game.m_gameplayTimer.getTimeScale();
						game.m_gameplayTimer.setTimeScale(debugPauseTime);
						debugPauseTime = currentScale;
					}
					return true;
					case Keyboard::m_Enter:
						step = true;
						break;
					case Keyboard::m_Backspace:
						stepBack = true;
						break;

					case Keyboard::m_L:
						game.m_window->lockCursor(true);
						break;
					case Keyboard::m_U:
						game.m_window->lockCursor(false);
						break;

					case Keyboard::m_X:
						generateMonster(game.m_monsters, game.m_randomGenerator, loc, 0);
						break;

					case Keyboard::m_Home:
					{
						auto p = game.m_camera.getPosition();
						p.z = -50;
						game.m_camera.setPosition(p);
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


					case Keyboard::m_F1:
						placePickup(game.m_pickups, loc, IncreasedMovementSpeed, WeaponTypeCount);
						return true;
					case Keyboard::m_F2:
						placePickup(game.m_pickups, loc, Heal, WeaponTypeCount);
						return true;
					case Keyboard::m_F3:
						placePickup(game.m_pickups, loc, IncreasedRateOfFire, WeaponTypeCount);
						return true;
					case Keyboard::m_F4:
						placePickup(game.m_pickups, loc, BonusTypeCount, Pistol);
						return true;
					case Keyboard::m_F5:
						placePickup(game.m_pickups, loc, BonusTypeCount, Shotgun);
						return true;
					case Keyboard::m_F6:
						placePickup(game.m_pickups, loc, BonusTypeCount, Uzi);
						return true;
					case Keyboard::m_F7:
						placePickup(game.m_pickups, loc, SlowTime, WeaponTypeCount);
						return true;
					case Keyboard::m_F8:
						placePickup(game.m_pickups, loc, BonusTypeCount, Sniper);
						break;
				}
			}
			return false;
		});
#endif
		game.m_messageHandlers.emplace_back([&](const WindowEvent& w)
		{
			if( w.m_type == WindowEventType::WE_KEYBOARDKEY && game.m_players.size() >= 1 )
			{
				auto firstDown = w.m_keyboard.m_isDown && !w.m_keyboard.m_previouslyDown;
				auto up = !w.m_keyboard.m_isDown && w.m_keyboard.m_previouslyDown;
				switch( w.m_keyboard.m_keyCode )
				{
					case Keyboard::m_W:
						if( firstDown )
							game.m_players[0].directions[Up] = true;
						if( up )
							game.m_players[0].directions[Up] = false;
						break;
					case Keyboard::m_S:
						if( firstDown )
							game.m_players[0].directions[Down] = true;
						if( up )
							game.m_players[0].directions[Down] = false;
						break;
					case Keyboard::m_A:
						if( firstDown )
							game.m_players[0].directions[Left] = true;
						if( up )
							game.m_players[0].directions[Left] = false;
						break;
					case Keyboard::m_D:
						if( firstDown )
							game.m_players[0].directions[Right] = true;
						if( up )
							game.m_players[0].directions[Right] = false;
						break;
					case Keyboard::m_E:
						if( firstDown )
							activateDefenseMatrix(game);
						break;
					case Keyboard::m_Q:
						if( firstDown )
							activateTimeCapsule(game);
						break;
					case Keyboard::m_F:
					{
						if( firstDown )
						{
							activateBlink(game);
						}
					} break;
				}
			}
			return false;
		});

		game.m_messageHandlers.emplace_back([&](const WindowEvent& w)
		{
			if( w.m_type == WindowEventType::WE_GAMEPADAXIS && game.m_players.size() > w.m_gamepadAxis.m_gamepad + 1U )
			{
				auto i = w.m_gamepadAxis.m_gamepad + 1;
				if( w.m_gamepadAxis.m_axis == Gamepad::m_LeftStick )
				{
					if( w.m_gamepadAxis.m_x > 0 )
					{
						game.m_players[i].directions[Right] = true;
						game.m_players[i].directions[Left] = false;
					}
					else if( w.m_gamepadAxis.m_x < 0 )
					{
						game.m_players[i].directions[Left] = true;
						game.m_players[i].directions[Right] = false;
					}
					else
					{
						game.m_players[i].directions[Left] = false;
						game.m_players[i].directions[Right] = false;
					}


					if( w.m_gamepadAxis.m_y > 0 )
					{
						game.m_players[i].directions[Up] = true;
						game.m_players[i].directions[Down] = false;
					}
					else if( w.m_gamepadAxis.m_y < 0 )
					{
						game.m_players[i].directions[Down] = true;
						game.m_players[i].directions[Up] = false;
					}
					else
					{
						game.m_players[i].directions[Up] = false;
						game.m_players[i].directions[Down] = false;
					}
					return true;
				}
				if( w.m_gamepadAxis.m_axis == Gamepad::m_RightStick )
				{
					game.m_players[i].aim = game.m_graphicsSystem.screenToWorld({(float)w.m_gamepadAxis.m_x, -(float)w.m_gamepadAxis.m_y}, game.m_camera);
					return true;
				}
			}
			if( w.m_type == WindowEventType::WE_GAMEPADBUTTON && game.m_players.size() > w.m_gamepadButton.m_gamepad + 1U )
			{
				auto i = w.m_gamepadButton.m_gamepad + 1;
				if( w.m_gamepadButton.m_isDown )
				{
					game.m_players[i].isShooting = true;
				}
				else
				{
					game.m_players[i].isShooting = false;
				}
				return true;
			}
			return false;
		});
	}
}
