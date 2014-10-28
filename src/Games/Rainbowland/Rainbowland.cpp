//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Rainbowland/Rainbowland.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Games/GameLoopParams.h>
#include <Graphics/Camera.h>
#include <Input/KeyCodes.h>
#include <Util/ResourceFile.h>
#include <Util/Utility.h>
#include <Window/Window.h>
/******* end headers *******/

namespace Core
{
	void generateBullets(std::vector<RayBullet>& bullets, uint32_t count, float spread, const Vec2& origin, const Vec2& direction)
	{
		for(uint32_t i = 0; i < count; ++i)
		{
			bullets.emplace_back(RayBullet{origin, origin, direction});
		}
	}

	void RainbowlandGame::shutdown()
	{
		DEBUG_INFO("---------------------------------");
		bool status = true;
		status &= m_textureCache.shutdown();
		status &= m_fontCache.shutdown();
		status &= m_inputSystem.shutdown();
		status &= m_graphicsSystem.shutdown();

		if(!status)
		{
			DEBUG_INFO("\nShutdown has failed! Bugs...");
		}
	}

	bool RainbowlandGame::init(Window& window)
	{
		m_window = &window;

		m_isRunning =
			m_graphicsSystem.init(m_fontCache, m_textureCache, window) &&
			m_inputSystem.init(window) &&
			m_fontCache.init(m_textureCache) &&
			m_imageCache.init(m_textureCache) &&
			m_textureCache.init(m_graphicsSystem) &&
			true;

		window.resize(800, 600);
		window.showCursor(true);

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
					float x = w.m_mouseButton.m_x;
					float y = w.m_mouseButton.m_y;
					x -= m_window->getSizeX() / 2;
					y -= m_window->getSizeY() / 2;
					x /= m_window->getSizeX();
					y /= m_window->getSizeX();
					generateBullets(m_rayBullets, 1, 0, m_players[0].transform.position, {x, -y}); //y is inverted because screen is 0,0 at top left, while the graphics are bottom left
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

		//player
		m_numPlayers = 1;
		m_players[0].transform.position.set(0, 0);
		m_players[0].transform.scale.set(1, 1);
		m_players[0].transform.rotation = 0;
		m_players[0].color.set(1, 0, 0);
		m_players[0].boundingBox.set(0, 0, 0.5f, 0.5f);

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

		for(uint32_t i = 0; i < m_rayBullets.size(); )
		{
			m_rayBullets[i].position += m_rayBullets[i].velocity;
			auto l = length(m_rayBullets[i].position - m_rayBullets[i].origin);
			
			if(l > 30)
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

		Camera camera;
		camera.setPosition({0, 0, -50});

		m_graphicsSystem.setPerspectiveProjection();
		m_graphicsSystem.applyCamera(camera);
		
		for(uint32_t i = 0; i < m_numPlayers; ++i)
		{
			m_graphicsSystem.drawQuad(m_players[i].transform, m_players[i].boundingBox.halfSize(), m_players[i].color);
		}

		for(auto& rayBullet : m_rayBullets)
		{
			Transform tf;
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

		//static Transform framerateTf;
		//framerateTf.position.set(0.5f*m_window->getSizeX() - 170, 0.5f*m_window->getSizeY() - 10);
		//framerateTf.scale.set(0.5f, 0.5f);
		//m_graphicsSystem.setOrthographicProjection();
		//m_graphicsSystem.clearCamera();
		//m_graphicsSystem.drawText(m_defaultFont, "ms per frame: " + std::to_string(Time::microsToMilis(m_framerateTimer.getDeltaMicros())), framerateTf, Color(0, 0, 0), 0, false);
		

		m_graphicsSystem.present();
	}
}
