//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Hedgehog/Hedgehog.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Games/GameLoopParams.h>
#include <Input/KeyCodes.h>
#include <Util/DataFile.h>
#include <Util/Utility.h>
#include <Window/Window.h>
#include <Window/WindowEvent.h>
/******* end headers *******/

namespace Core
{
	void HedgehogGame::shutdown()
	{
		DEBUG_INFO("---------------------------------");
		bool status = true;
		status &= m_animationCache.shutdown();
		status &= m_imageCache.shutdown();
		status &= m_scriptCache.shutdown();
		status &= m_spritesheetCache.shutdown();
		status &= m_textureCache.shutdown();
		status &= m_animation.shutdown();
		status &= m_graphics.shutdown();
		status &= m_input.shutdown();
		status &= m_scripter.shutdown();
		if(!status)
		{
			DEBUG_INFO("\nShutdown has failed! Bugs...");
		}
	}

	bool HedgehogGame::init(Window& window)
	{
		m_window = &window;

		window.resize(1024, 768);
		DEBUG_CODE_START
			window.openConsole(1050, 0);
		DEBUG_CODE_END;

		window.setGameTimer(m_logicTimer);

		m_logicTimer = window.getTimer();
		m_logicTimer.setTimeScale(Time::NORMAL_TIME);
		m_renderTimer.setTimeScale(Time::NORMAL_TIME);

		m_isRunning =
			//systems
			m_animation.init(m_animationCache) &&
			m_graphics.init(window) &&
			m_input.init(window) &&
			m_scripter.init() &&

			//caches
			m_animationCache.init() &&
			m_imageCache.init() &&
			m_scriptCache.init(m_scripter) &&
			m_spritesheetCache.init(m_animationCache, m_imageCache, m_textureCache) &&
			m_textureCache.init(m_graphics) &&
			
			//last statement is a fixed 'true' so all previous can have '&&' at the end
			true;


		if(m_isRunning)
		{
			m_isRunning &= m_scriptCache.loadFromFile(RESOURCE("Scripts/hedgehog_game.lua"), false);
			//m_isRunning &= m_scriptCache.loadFromFile(RESOURCE("Scripts/lib.lua"), false);
			m_isRunning &= m_scripter.functionExists("game_tick") && m_scripter.functionExists("game_render");
		}

		if(m_isRunning)
		{
			if(m_scripter.functionExists("game_init"))
			{
				if(m_scripter.doFunction("game_init", this, CLASS(HedgehogGame), 1))
				{
					m_isRunning &= m_scripter.toBool();
					m_scripter.pop();
				}
			}

			m_isRunning &= m_graphics.initVertexShader(RESOURCE("Shaders/shader.hlsl"));
			m_isRunning &= m_graphics.initPixelShader(RESOURCE("Shaders/shader.hlsl"));

			DataFile dataFile(m_scripter);
			if(dataFile.open(RESOURCE("Defs/font.sheet")))
			{
				m_isRunning &= m_graphics.initFont(dataFile);
				dataFile.close();
			}
			
			if(dataFile.open(RESOURCE("Defs/hedgehog.sheet")))
			{
				m_isRunning &= m_spritesheetCache.loadFromFile(dataFile, false);
				dataFile.close();
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
					this->onFileChanged(w.m_fileChange.m_index);
					return true;
				}
				return false;
			});


			m_player.m_animationData.m_animationID = m_animationCache.getAnimationID("walk");
			m_player.m_animationData.m_time = 0;
			m_animation.registerData(m_player.m_animationData);
		}
		m_framerateTimer.update();

		DEBUG_INFO("---------------------------------");
		return m_isRunning;
	}

	

	bool HedgehogGame::tick()
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
		m_framerateTimer.update();
		return m_isRunning;
	}

	
	
	bool HedgehogGame::tickLogic(uint64_t updateTime)
	{
		bool continueRunning = true;
		m_logicTimer.updateBy(updateTime);

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

		m_scripter.doFunction("game_tick", this, CLASS(HedgehogGame));

		return continueRunning;
	}

	void HedgehogGame::tickRender(uint64_t updateTime)
	{
		m_renderTimer.updateBy(updateTime);

		m_graphics.begin();

		static Transform framerateTf;
		
		framerateTf.position.set(-0.5f*m_window->getSizeX(), 0.5f*m_window->getSizeY());
		framerateTf.scale.set(0.6f, 0.6f);
		m_graphics.drawText("ms per frame: " + std::to_string(Time::microsToMilis(m_framerateTimer.getDeltaMicros())), framerateTf, Color(0,0,0), 0, false);

		m_scripter.doFunction("game_render", this, CLASS(HedgehogGame));

		m_graphics.present();
		
	}

	void HedgehogGame::onFileChanged(uint32_t index)
	{
		uint32_t action;
		std::string file;
		if(m_window->getChangedFile(index, action, file))
		{
			auto pos = file.find_last_of('.');
			auto ext = file.substr(pos + 1);
			if(ext == "lua")
			{
				if(m_scriptCache.loadFromFile(RESOURCE_S(file), true))
					DEBUG_INFO("Reloaded script ", file);
			}
			else if(ext == "sheet")
			{
				DataFile config(m_scripter);
				if(config.open(RESOURCE_S(file)))
				{
					if(m_spritesheetCache.loadFromFile(config, true))
						DEBUG_INFO("Reloaded spritesheet file ", file);
					config.close();
				}
			}
		}
	}
}
