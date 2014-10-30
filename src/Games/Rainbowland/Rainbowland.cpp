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
	void RainbowlandGame::shutdown()
	{
		DEBUG_INFO("---------------------------------");
		bool status = true;
		status &= m_textureCache.shutdown();
		status &= m_fontCache.shutdown();
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

		window.resize(1024, 768);
		window.showCursor(true);

		m_isRunning =
			m_graphicsSystem.init(m_fontCache, m_textureCache, window) &&
			m_inputSystem.init(window) &&
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
				if(w.m_type == WindowEventType::WE_MOUSEBUTTON && w.m_mouseButton.m_button == Mouse::m_LeftButton && w.m_mouseButton.m_isDown)
				{
					float x = (float)w.m_mouseButton.m_x;
					float y = (float)w.m_mouseButton.m_y;
					Vec2 normalized = m_graphicsSystem.screenToWorld({x, y}, m_camera);
					generateBullets(m_rayBullets, 1, 0, m_players[0].transform.position, normalized);
					return true;
				}
				return false;
			});

			m_messageHandlers.emplace_back([&](const WindowEvent& w)
			{
				if(w.m_type == WindowEventType::WE_MOUSEBUTTON && w.m_mouseButton.m_button == Mouse::m_RightButton && w.m_mouseButton.m_isDown)
				{
					float x = (float)w.m_mouseButton.m_x;
					float y = (float)w.m_mouseButton.m_y;
					Vec2 normalized = m_graphicsSystem.screenToWorld({x, y}, m_camera);
					generateBullets(m_rayBullets, 5, 0.5f, m_players[0].transform.position, normalized);
					return true;
				}
				return false;
			});

			m_messageHandlers.emplace_back([&](const WindowEvent& w)
			{
				if(w.m_type == WindowEventType::WE_KEYBOARDKEY && w.m_keyboard.m_isDown)
				{
					float spdAdd = 0.5f;
					if(w.m_keyboard.m_keyCode == Keyboard::m_W)
					{
						m_players[0].acceleration.y += spdAdd;
						m_players[0].acceleration.y = std::max(std::abs(m_players[0].acceleration.x), std::abs(m_players[0].acceleration.y));
						clamp(-1.0f, 1.0f, m_players[0].acceleration.y);
						return true;
					}
					else if(w.m_keyboard.m_keyCode == Keyboard::m_S)
					{
						m_players[0].acceleration.y -= spdAdd;
						m_players[0].acceleration.y = -std::max(std::abs(m_players[0].acceleration.x), std::abs(m_players[0].acceleration.y));
						clamp(-1.0f, 1.0f, m_players[0].acceleration.y);
						return true;
					}
					else if(w.m_keyboard.m_keyCode == Keyboard::m_A)
					{
						m_players[0].acceleration.x -= spdAdd;
						m_players[0].acceleration.x = -std::max(std::abs(m_players[0].acceleration.x), std::abs(m_players[0].acceleration.y));
						clamp(-1.0f, 1.0f, m_players[0].acceleration.x);
						return true;
					}
					else if(w.m_keyboard.m_keyCode == Keyboard::m_D)
					{
						m_players[0].acceleration.x += spdAdd;
						m_players[0].acceleration.x = std::max(std::abs(m_players[0].acceleration.x), std::abs(m_players[0].acceleration.y));
						clamp(-1.0f, 1.0f, m_players[0].acceleration.x);
						
						return true;
					}
				}
				if(w.m_type == WindowEventType::WE_KEYBOARDKEY && !w.m_keyboard.m_isDown)
				{
					if(w.m_keyboard.m_keyCode == Keyboard::m_W)
					{
						m_players[0].acceleration.y = 0;
						return true;
					}
					else if(w.m_keyboard.m_keyCode == Keyboard::m_S)
					{
						m_players[0].acceleration.y = 0;
						return true;
					}
					else if(w.m_keyboard.m_keyCode == Keyboard::m_A)
					{
						m_players[0].acceleration.x = 0;
						return true;
					}
					else if(w.m_keyboard.m_keyCode == Keyboard::m_D)
					{
						m_players[0].acceleration.x = 0;
						return true;
					}
				}
				return false;
			});

			m_isRunning &= m_graphicsSystem.initVertexShader("Shaders/shader.hlsl");
			m_isRunning &= m_graphicsSystem.initPixelShader("Shaders/shader.hlsl");
		}

		m_logicTimer.setTimeScale(Time::NORMAL_TIME);
		m_renderTimer.setTimeScale(Time::NORMAL_TIME);
		
		//background
		m_graphicsSystem.setBackgroundColor(0.5f, 0.5f, 0.5f);

		//players
		m_numPlayers = 1;
		uint32_t p = 0;
		m_players.emplace_back();
		m_players[p].transform.position.set(0, 0);
		m_players[p].transform.scale.set(1, 1);
		m_players[p].transform.rotation = 0;
		m_players[p].color.set(1, 0, 0);
		m_players[p].boundingBox.set(0, 0, 0.5f, 0.5f);
		m_players[p].velocity.set(5.0f, 5.0f);
		m_players[p].acceleration.set(0, 0);
		
		m_graphicsSystem.setPerspectiveProjection();
		
		m_camera.setPosition({-15.0f, 10.0f, -50.0f});
		m_graphicsSystem.applyCamera(m_camera);
		Vec2 topleft = m_graphicsSystem.screenToWorld({0, 0}, m_camera);
		m_playingField.halfWidth = std::abs(topleft.x);
		m_playingField.halfHeight = std::abs(topleft.y);

		m_camera.setPosition({0, 0, -50});

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
		auto& evs = m_inputSystem.getEvents();
		for(auto& e : evs)
		{
			for(auto& f : m_messageHandlers)
			{
				f(e);
			}
		}

		movePlayers(m_logicTimer, m_players, m_numPlayers, m_playingField);

		Vec2 averagePos;
		for(uint32_t i = 0; i < m_numPlayers; ++i)
		{
			averagePos += m_players[i].transform.position;
		}
		averagePos /= (float)m_numPlayers;
		auto pos = m_camera.getPosition();
		pos.x = averagePos.x;
		pos.y = averagePos.y;
		clamp(-15.0f, 15.0f, pos.x);
		clamp(-10.0f, 10.0f, pos.y);
		m_camera.setPosition(pos);

		for(uint32_t i = 0; i < m_rayBullets.size(); )
		{
			auto travel = Vec2::length(m_rayBullets[i].velocity);
			m_rayBullets[i].position += m_rayBullets[i].velocity;
			if(Vec2::length(m_rayBullets[i].position - m_rayBullets[i].origin) > 7)
			{
				m_rayBullets[i].origin += m_rayBullets[i].velocity;
			}
			m_rayBullets[i].travelled += travel;
			
			if(m_rayBullets[i].travelled > 50)
			{
				m_rayBullets[i] = m_rayBullets.back();
				m_rayBullets.pop_back();
			}
			else
			{
				++i;
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

		Transform tf;
		Vec2 p1{0, 0}, p2{0, 1}, p3{1, 0};
		m_graphicsSystem.drawLine(tf, p1, p2, {1, 1, 1});
		m_graphicsSystem.drawLine(tf, p1, p3, {1, 1, 1});

		Vec2 tl{-40, 30}, tr{40, 30}, bl{-40, -30}, br{40, -30};
		m_graphicsSystem.drawLine(tf, tl, tr, {1, 1, 1});
		m_graphicsSystem.drawLine(tf, tr, br, {1, 1, 1});
		m_graphicsSystem.drawLine(tf, br, bl, {1, 1, 1});
		m_graphicsSystem.drawLine(tf, bl, tl, {1, 1, 1});
		
		for(uint32_t i = 0; i < m_numPlayers; ++i)
		{
			m_graphicsSystem.drawQuad(m_players[i].transform, m_players[i].boundingBox.halfSize(), m_players[i].color);
		}

		for(auto& rayBullet : m_rayBullets)
		{
			m_graphicsSystem.drawLine(tf, rayBullet.origin, rayBullet.position, {1, 1, 1, 1});
		}

		/*
		auto lua = m_luaSystem.getStack();
		lua.pull("game_render", 0);
		if(lua.is<luaFunction>())
		{
			lua.call(luaCustom{this, CLASS(HedgehogGame)});
		}
		*/

		/*static Transform framerateTf;
		framerateTf.position.set(0.5f*m_window->getSizeX() - 170, 0.5f*m_window->getSizeY() - 10);
		framerateTf.scale.set(0.5f, 0.5f);
		m_graphicsSystem.setOrthographicProjection();
		m_graphicsSystem.clearCamera();
		m_graphicsSystem.drawText(m_defaultFont, "ms per frame: " + std::to_string(Time::microsToMilis(m_framerateTimer.getDeltaMicros())), framerateTf, Color(0, 0, 0), 0, false);*/
		

		m_graphicsSystem.present();
	}
}
