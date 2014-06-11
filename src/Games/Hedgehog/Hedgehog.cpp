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
		m_animation.releasePlayer(m_player.m_animationPlayerID);
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

		m_logicTimer.setTimeScale(Time::NORMAL_TIME);
		m_renderTimer.setTimeScale(Time::NORMAL_TIME);

		m_isRunning =
			//systems
			m_animation.init(m_animationCache) &&
			m_graphics.init(m_textureCache, window) &&
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

		m_camera.setPosition(Vec3(0, 0, -15));

		if(m_isRunning)
		{
			m_isRunning &= m_scriptCache.loadFromFile(RESOURCE("Scripts/hedgehog_game.lua"), false);
			//m_isRunning &= m_scriptCache.loadFromFile(RESOURCE("Scripts/lib.lua"), false);
			m_isRunning &= m_scripter.functionExists("game_tick") && m_scripter.functionExists("game_render");
		}

		if(m_isRunning)
		{
			m_isRunning &= m_graphics.initVertexShader(RESOURCE("Shaders/shader.hlsl"));
			m_isRunning &= m_graphics.initPixelShader(RESOURCE("Shaders/shader.hlsl"));

			DataFile dataFile(m_scripter);
			if(dataFile.open(RESOURCE("Defs/font.font")))
			{
				m_isRunning &= m_graphics.initFont(dataFile);
				dataFile.close();
			}
			
			if(dataFile.open(RESOURCE("Defs/hedgehog.sheet")))
			{
				m_isRunning &= m_spritesheetCache.loadFromFile(dataFile, false);
				dataFile.close();
			}

			if(dataFile.open(RESOURCE("Defs/apples.sheet")))
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

			if(m_scripter.functionExists("game_init"))
			{
				if(m_scripter.doFunction("game_init", this, CLASS(HedgehogGame), 1))
				{
					m_isRunning &= m_scripter.toBool();
					m_scripter.pop();
				}
			}

			m_player.m_animationPlayerID = m_animation.createPlayer(m_player.m_imageID);
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
		m_scripter.update();
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

		m_scripter.doFunction("game_render", this, CLASS(HedgehogGame));

		static Transform framerateTf;
		framerateTf.position.set(0.5f*m_window->getSizeX() - 20, 0.5f*m_window->getSizeY() - 10);
		framerateTf.scale.set(0.5f, 0.5f);
		m_graphics.setOrthographicProjection();
		m_graphics.clearCamera();
		m_graphics.drawText("ms per frame: " + std::to_string(Time::microsToMilis(m_framerateTimer.getDeltaMicros())), framerateTf, Color(0, 0, 0), 2, false);

		m_graphics.present();
	}



	uint32_t HedgehogGame::createProp()
	{
		return m_props.create();
	}

	Prop& HedgehogGame::getProp(uint32_t id)
	{
		return m_props.get(id);
	}

	void HedgehogGame::removeProp(uint32_t id)
	{
		m_props.remove(id);
	}

	void HedgehogGame::onFileChanged(uint32_t index)
	{
		uint32_t action;
		std::string file;
		if(m_window->getChangedFile(index, action, file))
		{
			auto pos = file.find_last_of('.');
			auto ext = file.substr(pos + 1);
			if(ext == "lua" && action == Core::FILE_MODIFIED)
			{
				if(m_scriptCache.loadFromFile(ResourcePath(file).c_str(), true))
					DEBUG_INFO("Reloaded script ", file);
			}
			else if(ext == "sheet" && action == Core::FILE_MODIFIED)
			{
				DataFile config(m_scripter);
				if(config.open(ResourcePath(file).c_str()))
				{
					if(m_spritesheetCache.loadFromFile(config, true))
						DEBUG_INFO("Reloaded spritesheet file ", file);
					config.close();
				}
			}
			else if(ext == "font" && action == Core::FILE_MODIFIED)
			{
				DataFile config(m_scripter);
				if(config.open(ResourcePath(file).c_str()))
				{
					if(m_graphics.initFont(config))
						DEBUG_INFO("Reloaded font file ", file);
					config.close();
				}
			}
			else if(ext == "hlsl" && action == Core::FILE_MODIFIED)
			{
				if(m_graphics.initVertexShader(ResourcePath(file).c_str()) &&
				   m_graphics.initPixelShader(ResourcePath(file).c_str()))
				{
					DEBUG_INFO("Reloaded shader file ", file);
				}
			}
			else if(ext == "tif" || ext == "png")
			{
				if(action == Core::FILE_MODIFIED)
				{
					if(m_textureCache.onFileModified(ResourcePath(file).c_str()))
						DEBUG_INFO("Reloaded texture file ", file);
					else
						DEBUG_INFO("Reload failed for texture file ", file);
				}
				else
				{
					DEBUG_INFO("Unsupported event type ", action, " for file ", file);
				}
			}
			else
			{
				DEBUG_INFO("No reload for file ", file);
			}
		}
	}
}
