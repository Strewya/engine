//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Hedgehog/Hedgehog.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Games/GameLoopParams.h>
#include <Input/KeyCodes.h>
#include <Util/Utility.h>
#include <Window/Window.h>
#include <Window/WindowEvent.h>
/******* end headers *******/

namespace Core
{
	bool HedgehogGame::init(Window& window)
	{
		m_window = &window;

		window.resize(1024, 768);
		DEBUG_LINE(window.openConsole(1050, 0));

		m_logicTimeScale = Time::NORMAL_TIME;

		m_isRunning = 
			m_scripter.init() &&
			m_scripter.scriptFileExists(RESOURCE("Scripts/hedgehog_game.lua")) &&
			m_input.init(window) &&
			m_animation.init(m_graphics) &&
			m_graphics.init(window);

		if(m_isRunning)
		{
			m_scripter.executeScriptFile(RESOURCE("Scripts/hedgehog_game.lua"));
			m_isRunning &= m_scripter.functionExists("game_init") && m_scripter.functionExists("game_tick") && m_scripter.functionExists("game_render");
		}
		if(m_isRunning)
		{
			m_scripter.executeFunction("game_init", this, CLASS(HedgehogGame));

			
			m_isRunning &= m_graphics.initVertexShader(RESOURCE("Shaders/shader.hlsl"));
			m_isRunning &= m_graphics.initPixelShader(RESOURCE("Shaders/shader.hlsl"));

			auto df = m_scripter.getDataFile();
			if(df.open(RESOURCE("Sheets/font.sheet")))
			{
				m_isRunning &= m_graphics.initFont(df);
				df.close();
			}
			if(df.open(RESOURCE("Sheets/hedgehog.sheet")))
			{
				m_isRunning &= m_graphics.initSpritesheet(df); 
				df.close();
			}

			m_messageHandlers.reserve(3);
			m_messageHandlers.emplace_back([&](const WindowEvent& w)
			{
				if(w.m_type == WindowEventType::WE_KEYBOARDKEY)
				{
					if(w.m_keyboard.m_keyCode == Keyboard::m_Escape)
					{
						m_window->close();
						m_isRunning = false;
						return true;
					}
				}
				return false;
			});

			m_messageHandlers.emplace_back([&](const WindowEvent& w)
			{
				if(w.m_type == WindowEventType::WE_FILECHANGE)
				{
					uint32_t action;
					std::string file;
					if(m_window->getChangedFile(w.m_fileChange.m_index, action, file))
					{
						auto pos = file.find_last_of('.');
						if(file.substr(pos + 1) == "lua")
						{
							DEBUG_INFO("Reloading script ", file);
							m_scripter.executeScriptFile(RESOURCE_S(file));
							return true;
						}
					}
				}
				return false;
			});


			m_player.m_animationData.m_animationID = m_graphics.getAnimationIndex("walk");
			m_player.m_animationData.m_time = 0;
			m_animation.registerData(m_player.m_animationData);
		}
		DEBUG_INFO("---------------------------------");
		return m_isRunning;
	}

	bool HedgehogGame::tick()
	{
		float fraction = 0;
		uint64_t unusedMicros = 0;
		static const uint64_t microsPerFrame = CORE_MICROS_PER_FRAME;

		for(uint32_t l = getLogicUpdateCount(m_logicTimer, microsPerFrame, fraction, unusedMicros); l--;)
		{
			if(!tickLogic(microsPerFrame))
			{
				m_isRunning = false;
				break;
			}
		}

		uint64_t fullUpdateTime = m_logicTimer.getLastRealTimeMicros() + unusedMicros - m_renderTimer.getLastRealTimeMicros();
		tickRender(fullUpdateTime);

		return m_isRunning;
	}

	bool HedgehogGame::shutdown()
	{
		DEBUG_INFO("---------------------------------");
		m_input.shutdown();
		m_scripter.shutdown();
		m_animation.shutdown();
		m_graphics.shutdown();
		return true;
	}
	
	bool HedgehogGame::tickLogic(uint64_t updateTime)
	{
		bool continueRunning = true;
		m_logicTimer.updateBy(updateTime, m_logicTimeScale);

		m_input.update(m_logicTimer);
		auto& evs = m_input.getEvents();
		for(auto& e : evs)
		{
			for(auto& f : m_messageHandlers)
			{
				f(e);
			}
		}

		m_animation.update(m_logicTimer.getDeltaMicros());

		m_scripter.executeFunction("game_tick", this, CLASS(HedgehogGame));
		return continueRunning;
	}

	void HedgehogGame::tickRender(uint64_t updateTime)
	{
		m_renderTimer.updateBy(updateTime, Time::NORMAL_TIME);

		m_graphics.begin();

		m_scripter.executeFunction("game_render", this, CLASS(HedgehogGame));

		m_graphics.present();
		
	}
}
