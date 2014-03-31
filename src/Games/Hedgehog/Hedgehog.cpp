//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Hedgehog/Hedgehog.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Games/GameLoopParams.h>
#include <Input/KeyCodes.h>
#include <Util/ConfigFile.h>
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
		status &= m_textureCache.shutdown();
		status &= m_animationCache.shutdown();
		status &= m_spritesheetCache.shutdown();
		status &= m_imageCache.shutdown();
		status &= m_input.shutdown();
		status &= m_scripter.shutdown();
		status &= m_animation.shutdown();
		status &= m_graphics.shutdown();
		if(!status)
		{
			DEBUG_INFO("\nShutdown has failed! Bugs...");
		}
	}

	bool HedgehogGame::init(Window& window)
	{
		m_window = &window;

		window.resize(1024, 768);
		DEBUG_CODE(
			window.openConsole(1050, 0);
		)

		m_logicTimer.setTimeScale(Time::NORMAL_TIME);
		m_renderTimer.setTimeScale(Time::NORMAL_TIME);

		m_isRunning =
			//systems
			m_scripter.init() &&
			m_input.init(window) &&
			m_animation.init(m_animationCache) &&
			m_graphics.init(window) &&
			//caches
			m_textureCache.init(m_graphics) &&
			m_imageCache.init() &&
			m_spritesheetCache.init(m_textureCache, m_imageCache) &&
			m_animationCache.init(m_spritesheetCache, m_imageCache) &&
			
			//last statement is a fixed 'true' so all previous can have '&&' at the end
			true;


		if(m_isRunning)
		{
			m_isRunning &= m_scripter.doFile(RESOURCE("Scripts/hedgehog_game.lua"));
			m_isRunning &= m_scripter.functionExists("game_tick") && m_scripter.functionExists("game_render");
		}

		if(m_isRunning)
		{	
			m_isRunning &= m_graphics.initVertexShader(RESOURCE("Shaders/shader.hlsl"));
			m_isRunning &= m_graphics.initPixelShader(RESOURCE("Shaders/shader.hlsl"));

			ConfigFile config(m_scripter);
			if(config.open(RESOURCE("Defs/font.sheet")))
			{
				m_isRunning &= m_graphics.initFont(config);
				config.close();
			}
			
			if(config.open(RESOURCE("Defs/hedgehog.sheet")))
			{
				m_isRunning &= m_spritesheetCache.loadSpritesheet(config);
				config.close();
			}

			if(config.open(RESOURCE("Defs/hedgehog.anim")))
			{
				m_isRunning &= m_animationCache.loadAnimations(config, true);
				config.close();
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
				if(m_scripter.doFile(RESOURCE_S(file)))
					DEBUG_INFO("Reloaded script ", file);
			}
			else if(ext == "anim")
			{
				ConfigFile config(m_scripter);
				if(config.open(RESOURCE_S(file)))
				{
					if(m_animationCache.loadAnimations(config, true))
						DEBUG_INFO("Reloaded animation file ", file);
				}
			}
			else if(ext == "sheet")
			{
				ConfigFile config(m_scripter);
				if(config.open(RESOURCE_S(file)))
				{
					if(m_spritesheetCache.reloadSpritesheet(config))
						DEBUG_INFO("Reloaded spritesheet file ", file);
				}
			}
		}
	}
}
