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
#include <Util/Random.h>
#include <Util/ResourceFile.h>
#include <Util/Utility.h>
#include <Window/Window.h>
/******* end headers *******/

namespace Core
{
	void generateBullets(std::vector<RayBullet>& bullets, uint32_t count, float spread, const Vec2& origin, const Vec2& direction)
	{

		Vec2 target = Vec2::normalize(origin + direction) * 10;
		Random gen(Time::countMilisInMicros(Time::getRealTimeMicros()));
		for(uint32_t i = 0; i < count; ++i)
		{
			Vec2 t{gen.randFloat()*spread * 2 - spread, gen.randFloat()*spread * 2 - spread};
			bullets.emplace_back(RayBullet{origin, origin, Vec2::normalize(target+t-origin)});
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

		window.resize(1024, 768);
		window.showCursor(true);

		m_isRunning =
			m_graphicsSystem.init(m_fontCache, m_textureCache, window) &&
			m_inputSystem.init(window) &&
			m_fontCache.init(m_textureCache) &&
			m_imageCache.init(m_textureCache) &&
			m_textureCache.init(m_graphicsSystem) &&
			true;

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
					Vec2 normalized = Vec2::normalize({x, -y}); //y is inverted because screen is 0,0 at top left, while the graphics are bottom left
					generateBullets(m_rayBullets, 1, 0, m_players[0].transform.position, normalized);
					return true;
				}
				return false;
			});

			m_messageHandlers.emplace_back([&](const WindowEvent& w)
			{
				if(w.m_type == WindowEventType::WE_MOUSEBUTTON && w.m_mouseButton.m_button == Mouse::m_RightButton && w.m_mouseButton.m_isDown)
				{
					float x = w.m_mouseButton.m_x;
					float y = w.m_mouseButton.m_y;
					x -= m_window->getSizeX() / 2;
					y -= m_window->getSizeY() / 2;
					Vec2 normalized = Vec2::normalize({x, -y}); //y is inverted because screen is 0,0 at top left, while the graphics are bottom left
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

		//player
		m_numPlayers = 2;
		uint32_t p = 0;
		m_players[p].transform.position.set(0, 0);
		m_players[p].transform.scale.set(1, 1);
		m_players[p].transform.rotation = 0;
		m_players[p].color.set(1, 0, 0);
		m_players[p].boundingBox.set(0, 0, 0.5f, 0.5f);
		m_players[p].velocity.set(0.1f, 0.1f);
		m_players[p].acceleration.set(0, 0);
		++p;
		m_players[p].transform.position.set(3, 3);
		m_players[p].transform.scale.set(1, 1);
		m_players[p].transform.rotation = 0;
		m_players[p].color.set(1, 1, 0);
		m_players[p].boundingBox.set(0, 0, 0.5f, 0.5f);
		m_players[p].velocity.set(0.1f, 0.1f);
		m_players[p].acceleration.set(0, 0);

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

		for(uint32_t i = 0; i < m_numPlayers; ++i)
		{
			Player& player = m_players[i];
			player.transform.position += (player.velocity*player.acceleration);
		}

		for(uint32_t i = 0; i < m_rayBullets.size(); )
		{
			auto travel = Vec2::length(m_rayBullets[i].velocity);
			m_rayBullets[i].position += m_rayBullets[i].velocity;
			if(Vec2::length(m_rayBullets[i].position - m_rayBullets[i].origin) > 7)
			{
				m_rayBullets[i].origin += m_rayBullets[i].velocity;
			}
			m_rayBullets[i].travelled += travel;
			
			if(m_rayBullets[i].travelled > 30)
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
		camera.setPosition({0, 0, -30});

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
