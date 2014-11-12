//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Rainbowland/Rainbowland.h>
/******* C++ headers *******/
#include <algorithm>
/******* extra headers *******/
#include <Games/GameLoopParams.h>
#include <Input/KeyCodes.h>
#include <Util/Random.h>
#include <Util/ResourceFile.h>
#include <Util/Utility.h>
#include <Window/Window.h>
/******* end headers *******/

namespace Core
{
#ifdef _DEBUG
	bool g_spawnEnabled = true;
	uint32_t mx = 0;
	uint32_t my = 0;
#endif

	void RainbowlandGame::shutdown()
	{
		DEBUG_INFO("---------------------------------");
		bool status = true;
		status &= m_textureCache.shutdown();
		status &= m_fontCache.shutdown();
		status &= m_guiSystem.shutdown();
		status &= m_luaSystem.shutdown();
		status &= m_inputSystem.shutdown();
		status &= m_graphicsSystem.shutdown();
		m_window->setFullscreen(false);
		if(!status)
		{
			DEBUG_INFO("\nShutdown has failed! Bugs...");
		}
	}

	bool RainbowlandGame::init(Window& window)
	{
		m_window = &window;

		window.resize(1200, 900);
		window.showCursor(true);
		
		m_isRunning =
			m_graphicsSystem.init(m_fontCache, m_textureCache, window) &&
			m_inputSystem.init(window) &&
			m_luaSystem.init() &&
			m_guiSystem.init({(float)window.getSizeX(), (float)window.getSizeY()});
			m_fontCache.init(m_textureCache) &&
			m_imageCache.init(m_textureCache) &&
			m_textureCache.init(m_graphicsSystem) &&
			true;

		window.setFullscreen(true);

		if(m_isRunning)
		{
			m_messageHandlers.emplace_back([&](const WindowEvent& w)
			{
				if(w.m_type == WindowEventType::WE_KEYBOARDKEY && w.m_keyboard.m_keyCode == Keyboard::m_Escape)
				{
					m_isRunning = false;
					return true;
				}
				return false;
			});

			m_messageHandlers.emplace_back([&](const WindowEvent& w)
			{
				if(w.m_type == WindowEventType::WE_MOUSEMOVE && !w.m_mouseMove.m_isRelative && m_players.size() >= 1)
				{
					m_players[0].aim.set((float)w.m_mouseMove.m_x, (float)w.m_mouseMove.m_y);
#ifdef _DEBUG
					mx = w.m_mouseMove.m_x;
					my = w.m_mouseMove.m_y;
#endif
					return true;
				}
				return false;
			});

			m_messageHandlers.emplace_back([&](const WindowEvent& w)
			{
				if(!perkMode && w.m_type == WindowEventType::WE_MOUSEBUTTON)
				{
					if(w.m_mouseButton.m_button == Mouse::m_LeftButton && m_players.size() >= 1)
					{
						m_players[0].aim.set((float)w.m_mouseButton.m_x, (float)w.m_mouseButton.m_y);
						if(w.m_mouseButton.m_isDown)
						{
							m_players[0].isShooting = true;
						}
						else
						{
							m_players[0].isShooting = false;
						}
						return true;
					}
					else if(w.m_mouseButton.m_button == Mouse::m_RightButton && w.m_mouseButton.m_isDown && m_players.size() >= 1)
					{
						return enterPerkMode(*this);
					}
				}
				return false;
			});
#ifdef _DEBUG
			m_messageHandlers.emplace_back([&](const WindowEvent& w)
			{
				if(!perkMode && w.m_type == WindowEventType::WE_KEYBOARDKEY && w.m_keyboard.m_isDown && !w.m_keyboard.m_previouslyDown && m_players.size() >= 1)
				{
					switch(w.m_keyboard.m_keyCode)
					{
						case Keyboard::m_1:
							selectWeapon(m_players[0], Pistol, m_weaponDatabase);
							return true;

						case Keyboard::m_2:
							selectWeapon(m_players[0], Shotgun, m_weaponDatabase);
							return true;

						case Keyboard::m_3:
							selectWeapon(m_players[0], Uzi, m_weaponDatabase);
							return true;

						case Keyboard::m_R:
							m_monsters.clear();
							g_spawnEnabled = !g_spawnEnabled;
							break;
					}
				}
				return false;
			});

			m_messageHandlers.emplace_back([&](const WindowEvent& w)
			{
				if(!perkMode && w.m_type == WindowEventType::WE_KEYBOARDKEY && w.m_keyboard.m_isDown && !w.m_keyboard.m_previouslyDown)
				{
					Random gen(Time::microsToMilis(Time::getRealTimeMicros()));
					Vec2 loc = m_graphicsSystem.screenToWorld({(float)mx, (float)my}, m_camera);
					switch(w.m_keyboard.m_keyCode)
					{
						case Keyboard::m_Space:
							if(m_gameplayTimer.getTimeScale() > 0.5f)
							{
								m_gameplayTimer.setTimeScale(Time::STOP_TIME);
							}
							else
							{
								m_gameplayTimer.setTimeScale(Time::NORMAL_TIME);
							}
							return true;

						case Keyboard::m_F1:
							placeBonus(m_bonuses, gen, loc, EffectType::IncreasedMovementSpeed);
							return true;
						case Keyboard::m_F2:
							placeBonus(m_bonuses, gen, loc, EffectType::Heal);
							return true;
						case Keyboard::m_F3:
							placeBonus(m_bonuses, gen, loc, EffectType::IncreasedRateOfFire);
							return true;
						case Keyboard::m_F4:
							placeBonus(m_bonuses, gen, loc, EffectType::WeaponDrop_Pistol);
							return true;
						case Keyboard::m_F5:
							placeBonus(m_bonuses, gen, loc, EffectType::WeaponDrop_Shotgun);
							return true;
						case Keyboard::m_F6:
							placeBonus(m_bonuses, gen, loc, EffectType::WeaponDrop_Uzi);
							return true;
						case Keyboard::m_F7:
							placeBonus(m_bonuses, gen, loc, EffectType::SlowTime);
							return true;

					}
				}
				return false;
			});
#endif
			m_messageHandlers.emplace_back([&](const WindowEvent& w)
			{
				if(w.m_type == WindowEventType::WE_KEYBOARDKEY && w.m_keyboard.m_isDown && !w.m_keyboard.m_previouslyDown && m_players.size() >= 1)
				{
					if(w.m_keyboard.m_keyCode == Keyboard::m_W)
					{
						m_players[0].directions[Up] = true;
						return true;
					}
					else if(w.m_keyboard.m_keyCode == Keyboard::m_S)
					{
						m_players[0].directions[Down] = true;
						return true;
					}
					else if(w.m_keyboard.m_keyCode == Keyboard::m_A)
					{
						m_players[0].directions[Left] = true;
						return true;
					}
					else if(w.m_keyboard.m_keyCode == Keyboard::m_D)
					{
						m_players[0].directions[Right] = true;
						return true;
					}
				}
				if(!perkMode && w.m_type == WindowEventType::WE_KEYBOARDKEY && !w.m_keyboard.m_isDown && w.m_keyboard.m_previouslyDown && m_players.size() >= 1)
				{
					if(w.m_keyboard.m_keyCode == Keyboard::m_W)
					{
						m_players[0].directions[Up] = false;
						return true;
					}
					else if(w.m_keyboard.m_keyCode == Keyboard::m_S)
					{
						m_players[0].directions[Down] = false;
						return true;
					}
					else if(w.m_keyboard.m_keyCode == Keyboard::m_A)
					{
						m_players[0].directions[Left] = false;
						return true;
					}
					else if(w.m_keyboard.m_keyCode == Keyboard::m_D)
					{
						m_players[0].directions[Right] = false;
						return true;
					}
				}
				return false;
			});

			m_isRunning &= m_graphicsSystem.initVertexShader("Shaders/shader.hlsl");
			m_isRunning &= m_graphicsSystem.initPixelShader("Shaders/shader.hlsl");

			m_isRunning &= (bool)m_textureCache.load("Textures/font_t.png");
			m_isRunning &= (bool)m_fontCache.load("Defs/font.font", m_luaSystem.getStack());
			m_defaultFont = m_fontCache.getResourceID("font");
		}

		m_logicTimer.setTimeScale(Time::NORMAL_TIME);
		m_renderTimer.setTimeScale(Time::NORMAL_TIME);
		
		initGame(*this);

		DEBUG_INFO("---------------------------------");
		return m_isRunning;
	}

	bool RainbowlandGame::tick()
	{
		float fraction = 0;
		uint64_t unusedMicros = 0;
		uint64_t droppedTime = 0;
		static const uint64_t microsPerFrame = CORE_MICROS_PER_FRAME;

		for(uint32_t l = getLogicUpdateCount(m_logicTimer, microsPerFrame, fraction, unusedMicros, droppedTime); l--;)
		{
			if(!tickLogic(microsPerFrame))
			{
				m_isRunning = false;
				break;
			}
		}
		m_logicTimer.updateBy(droppedTime);

		uint64_t fullUpdateTime = m_logicTimer.getLastRealTimeMicros() + unusedMicros - m_renderTimer.getLastRealTimeMicros();
		tickRender(fullUpdateTime);
		
		return m_isRunning;
	}

	bool RainbowlandGame::tickLogic(uint64_t updateTime)
	{
		bool continueRunning = true;
		m_logicTimer.updateBy(updateTime);

		m_inputSystem.update(m_logicTimer);
		auto evs = m_inputSystem.getEvents();
		for(auto& e : evs)
		{
			if(!m_guiSystem.handleEvent(e))
			{
				for(auto& f : m_messageHandlers)
				{
					f(e);
				}
			}
		}

		m_gameplayTimer.updateBy(m_logicTimer.getDeltaMicros());

		movePlayers(m_gameplayTimer, m_players, m_playingField);
#ifdef _DEBUG
		if(g_spawnEnabled)
#endif
		updateMonsterSpawners(m_gameplayTimer, m_monsterSpawners, m_monsters, m_players.size());
		moveMonsters(m_gameplayTimer, m_monsters, m_players);
		checkMonsterHurtingPlayer(m_gameplayTimer, m_monsters, m_players);
		checkLevelup(m_players, *this);
		
		if(m_players.size() > 0)
		{
			Vec2 averagePos;
			for(auto& player : m_players)
			{
				averagePos += player.transform.position;
			}
			averagePos /= (float)m_players.size();
			auto pos = m_camera.getPosition();
			pos.x = averagePos.x;
			pos.y = averagePos.y;
			clamp(-5.0f, 5.0f, pos.x);
			clamp(-2.0f, 2.0f, pos.y);
			m_camera.setPosition(pos);
		}
		for(auto& player : m_players)
		{
			fireWeapon(m_gameplayTimer, player, m_rayBullets, m_graphicsSystem, m_camera);
		}

		updatePerks(*this);
		moveBullets(m_gameplayTimer, m_rayBullets);
		checkBulletHits(m_rayBullets, m_monsters);
		VKillLocations locations;
		killMonsters(m_monsters, locations, m_players);
		generateBonuses(locations, m_bonuses);
		checkBonusPickup(m_players, *this);
		updateBonuses(m_gameplayTimer, *this);
		updateBonusEffects(m_gameplayTimer, *this);

		checkPlayerDeath(m_players);

		if(m_players.size() == 0)
		{
			m_deathTimer.updateBy(m_logicTimer.getDeltaMicros());
			if(m_deathTimer.getCurMicros() > Time::secondsToMicros(5))
			{
				cleanGame(*this);
				initGame(*this);
			}
		}

		/*
		auto lua = m_luaSystem.getStack();
		lua.pull("game_tick", 0);
		if(lua.is<luaFunction>())
		{
			auto called = lua.call(luaCustom{this, CLASS(HedgehogGame)}, &continueRunning);
			if(!called)
			{
				DEBUG_INFO(lua.to<std::string>());
				lua.pop();
				continueRunning = called;
			}
		}
		*/
		return continueRunning;
	}

	void RainbowlandGame::tickRender(uint64_t updateTime)
	{
		m_renderTimer.updateBy(updateTime);

		m_graphicsSystem.begin();
	
		m_graphicsSystem.setPerspectiveProjection();
		m_graphicsSystem.applyCamera(m_camera);
		
/*
		Transform tf;
		Vec2 p1{0, 0}, p2{0, 1}, p3{1, 0};
		m_graphicsSystem.drawLine(tf, p1, p2, {1, 1, 1});
		m_graphicsSystem.drawLine(tf, p1, p3, {1, 1, 1});*/
		
		for(auto& obj : m_players)
		{
			m_graphicsSystem.drawQuad(obj.transform, {obj.collisionData.radius, obj.collisionData.radius}, obj.color);
			if(obj.currentWeapon.ammo == 0)
			{
				auto textTf = obj.transform;
				textTf.scale.set(0.03f, 0.03f);
				m_graphicsSystem.drawText(m_defaultFont, "RELOADING", textTf, {0, 0, 0}, 1, false);
				textTf.position.y -= 1;
				auto timeRemaining = Time::microsToSeconds(obj.currentWeapon.reloadDelay - obj.weaponTimer.getCurMicros());
				m_graphicsSystem.drawText(m_defaultFont, std::to_string(timeRemaining), textTf, {0, 0, 0}, 1, false);
			}
		}

		for(auto& obj : m_monsterSpawners)
		{
			m_graphicsSystem.drawQuad(obj.transform, {1, 1}, {1, 0, 1});
		}

		for(auto& obj : m_monsters)
		{
			Color c = obj.color;
			c.a = 0.2f + (float)obj.health / (float)obj.maxHealth;
			m_graphicsSystem.drawQuad(obj.transform, {obj.collisionData.radius, obj.collisionData.radius}, c);
			Transform t;
			t.position = obj.transform.position;
			t.scale.set(0.03f, 0.03f);
			m_graphicsSystem.drawText(m_defaultFont, std::to_string(obj.health), t, {1, 1, 1}, 1, false);
		}

		for(auto& obj : m_bonuses)
		{
			auto c = obj.color;
			auto diff = obj.duration - obj.timer.getCurMicros();
			c.a = 0.2f + (float)diff / (float)obj.duration;
			m_graphicsSystem.drawQuad(obj.transform, {obj.collisionData.radius, obj.collisionData.radius}, c);
			std::string text = std::to_string(static_cast<int32_t>(Time::microsToSeconds(diff)+1));
			switch(obj.effect)
			{
				case WeaponDrop_Pistol:
					text += " P";
					break;
				case WeaponDrop_Shotgun:
					text += " S";
					break;
				case WeaponDrop_Uzi:
					text += " U";
					break;
			}
			auto textTf = obj.transform;
			textTf.scale.set(0.03f, 0.03f);
			m_graphicsSystem.drawText(m_defaultFont, text, textTf, {0,0,0}, 1, false);
		}

		for(auto& obj : m_rayBullets)
		{
			m_graphicsSystem.drawLine({}, obj.origin, {1, 1, 1, 0}, obj.position, {1, 1, 1, 1});
		}

		//****************************
		//			gui from now on
		//****************************
		m_graphicsSystem.clearCamera();
		m_graphicsSystem.setOrthographicProjection();
		if(m_players.size() == 0)
		{
			m_graphicsSystem.drawText(m_defaultFont, "HAHA YOU ARE DEAD", {}, {}, 1, false);
		}

		Transform tf;
		tf.position.set(5-0.5f*m_window->getSizeX(), 0.5f*m_window->getSizeY() - 20);
		tf.scale.set(0.75f, 0.75f);
		for(auto& player : m_players)
		{
			auto str = "Health: " + std::to_string(player.health) + "/" + std::to_string(player.maxHealth);
			str += "   Exp/next level: " + std::to_string(player.experience) + "/" + std::to_string(player.experienceForNextLevel);
			str += "   Ammo: " + std::to_string(player.currentWeapon.ammo) + "/" + std::to_string(player.currentWeapon.maxAmmo);
			str += "   Weapon: " + player.currentWeapon.name;
			m_graphicsSystem.drawText(m_defaultFont, str, tf, {0,0,0}, 0, false);
			tf.position.y += 20;
		}
		tf.position.set(10-(float)m_window->getSizeX()*0.5f, 300);
		for(auto& player : m_players)
		{
			for(auto& b : player.bonuses)
			{
				std::string str;
				switch(b.type)
				{
					case IncreasedMovementSpeed:
						str = "Runner: ";
						break;
					case IncreasedRateOfFire:
						str = "Shooter: ";
						break;
				}
				auto remaining = b.duration - b.timer.getCurMicros();
				str += std::to_string(static_cast<uint32_t>(Time::microsToSeconds(remaining) + 1));
				m_graphicsSystem.drawText(m_defaultFont, str, tf, {0, 0, 0}, 0, false);
				tf.position.y += 20;
			}
		}
		m_graphicsSystem.drawText(m_defaultFont, std::to_string(m_gameplayTimer.getTimeScale()), tf, {0, 0, 0}, 0, false);

		m_guiSystem.draw(m_graphicsSystem);

		m_graphicsSystem.setPerspectiveProjection();
		m_graphicsSystem.applyCamera(m_camera);

		


		/*
		static Transform framerateTf;
		framerateTf.position.set(0.5f*m_window->getSizeX() - 170, 0.5f*m_window->getSizeY() - 10);
		framerateTf.scale.set(0.5f, 0.5f);
		m_graphicsSystem.setOrthographicProjection();
		m_graphicsSystem.clearCamera();
		m_graphicsSystem.drawText(m_defaultFont, "ms per frame: " + std::to_string(Time::microsToMilis(m_logicTimer.getDeltaMicros())), framerateTf, Color(0, 0, 0), 0, false);
		*/

		m_graphicsSystem.present();
	}
}
