//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Hedgehog/Hedgehog.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Games/GameLoopParams.h>
#include <Input/KeyCodes.h>
#include <Scripting/LuaStack.h>
#include <Util/ResourceFile.h>
#include <Util/Utility.h>
#include <Window/Window.h>
#include <Window/WindowEvent.h>
/******* end headers *******/

/******* TESTING ***********/

/******* END TESTING *******/

namespace Core
{
	void HedgehogGame::shutdown()
	{
		DEBUG_INFO("---------------------------------");
		bool status = true;
		m_animationSystem.releasePlayer(m_player.m_animationPlayerID);
		status &= m_animationCache.shutdown();
		status &= m_loadHandlerCache.shutdown();
		status &= m_reloadHandlerCache.shutdown();
		status &= m_unloadHandlerCache.shutdown();
		status &= m_fontCache.shutdown();
		status &= m_imageCache.shutdown();
		status &= m_packageCache.shutdown();
		status &= m_scriptCache.shutdown();
		status &= m_textureCache.shutdown();
		status &= m_packageLoader.shutdown();
		status &= m_animationSystem.shutdown();
		status &= m_graphicsSystem.shutdown();
		status &= m_inputSystem.shutdown();
		status &= m_luaSystem.shutdown();
		if( !status )
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

		//runPlayground();

		m_isRunning =
			//systems
			m_animationSystem.init(m_animationCache) &&
			m_graphicsSystem.init(m_fontCache, m_textureCache, window) &&
			m_inputSystem.init(window) &&
			m_luaSystem.init() &&

			//caches
			m_animationCache.init(m_imageCache) &&
			m_loadHandlerCache.init("Loaded file", "File not loaded") &&
			m_reloadHandlerCache.init("Reloaded file", "File not reloaded") &&
			m_unloadHandlerCache.init("Unloaded file", "File not unloaded") &&
			m_fontCache.init(m_textureCache) &&
			m_imageCache.init(m_textureCache) &&
			m_packageCache.init() &&
			m_scriptCache.init(m_luaSystem) &&
			m_textureCache.init(m_graphicsSystem) &&

			//loaders
			m_packageLoader.init(m_packageCache, m_loadHandlerCache, m_unloadHandlerCache) &&

			//last statement is a fixed 'true' so all previous can have '&&' at the end
			true;

		m_camera.setPosition(Vec3(0, 0, -15));

		if( m_isRunning )
		{
			m_loadHandlerCache.registerHandler("tif", std::bind(&TextureCache::load, &m_textureCache, std::placeholders::_1));
			m_loadHandlerCache.registerHandler("png", std::bind(&TextureCache::load, &m_textureCache, std::placeholders::_1));
			m_loadHandlerCache.registerHandler("lua", std::bind(&ScriptCache::load, &m_scriptCache, std::placeholders::_1));
			m_loadHandlerCache.registerHandler("font", std::bind(&FontCache::load, &m_fontCache, std::placeholders::_1, m_luaSystem.getStack()));
			m_loadHandlerCache.registerHandler("sheet", [&](const ResourceFile& f)
			{
				auto res = m_imageCache.load(f, m_luaSystem.getStack());
				if(res) res = m_animationCache.load(f, m_luaSystem.getStack());
				return res;
			});

			m_reloadHandlerCache.registerHandler("tif", std::bind(&TextureCache::reload, &m_textureCache, std::placeholders::_1));
			m_reloadHandlerCache.registerHandler("png", std::bind(&TextureCache::reload, &m_textureCache, std::placeholders::_1));
			m_reloadHandlerCache.registerHandler("lua", std::bind(&ScriptCache::reload, &m_scriptCache, std::placeholders::_1));
			m_reloadHandlerCache.registerHandler("font", std::bind(&FontCache::reload, &m_fontCache, std::placeholders::_1, m_luaSystem.getStack()));
			m_reloadHandlerCache.registerHandler("sheet", [&](const ResourceFile& f)
			{
				auto res = m_imageCache.reload(f, m_luaSystem.getStack());
				if( res ) res = m_animationCache.reload(f, m_luaSystem.getStack());
				return res;
			});

			m_unloadHandlerCache.registerHandler("tif", std::bind(&TextureCache::unload, &m_textureCache, std::placeholders::_1));
			m_unloadHandlerCache.registerHandler("png", std::bind(&TextureCache::unload, &m_textureCache, std::placeholders::_1));
			m_unloadHandlerCache.registerHandler("lua", std::bind(&ScriptCache::unload, &m_scriptCache, std::placeholders::_1));
			m_unloadHandlerCache.registerHandler("font", std::bind(&FontCache::unload, &m_fontCache, std::placeholders::_1));
			m_unloadHandlerCache.registerHandler("sheet", [&](const ResourceFile& f)
			{
				auto res = m_imageCache.unload(f);
				if( res ) res = m_animationCache.unload(f);
				return res;
			});
		}

		if( m_isRunning )
		{
			auto& pkg = m_packageCache.getPackage("base");
			pkg.addFile("Scripts/hedgehog_game.lua");
			m_isRunning &= m_packageLoader.loadPackage("base");
		}

		if( m_isRunning )
		{
			auto lua = m_luaSystem.getStack();
			lua.pull("game_tick", 0);
			m_isRunning &= lua.isFunction();
			lua.pop();
			lua.pull("game_render", 0);
			m_isRunning &= lua.isFunction();
			lua.pop();
		}

		if( m_isRunning )
		{
			auto lua = m_luaSystem.getStack();

			m_isRunning &= m_graphicsSystem.initVertexShader("Shaders/shader.hlsl");
			m_isRunning &= m_graphicsSystem.initPixelShader("Shaders/shader.hlsl");
			
			m_messageHandlers.reserve(3);

			m_messageHandlers.emplace_back([&](const WindowEvent& w)
			{
				if( w.m_type == WindowEventType::WE_FILECHANGE )
				{
					uint32_t action;
					std::string file;
					if( m_window->getChangedFile(w.m_fileChange.m_index, action, file) && action == Core::FILE_MODIFIED )
					{
						m_reloadHandlerCache.dispatch(file);
						return true;
					}
				}
				return false;
			});

			m_player.m_animationPlayerID = m_animationSystem.createPlayer(m_player.m_imageID);

			lua.pull("game_init", 0);
			if( lua.isFunction() )
			{
				bool called = lua.call(CustomType{this, CLASS(HedgehogGame)}, &m_isRunning);
				m_isRunning &= called;
				if( !called )
				{
					DEBUG_INFO(lua.toString());
					lua.pop();
				}
			}
			lua.dumpStack();

			
			m_defaultFont = m_fontCache.getResourceID("font");
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

		for( uint32_t l = getLogicUpdateCount(m_logicTimer, microsPerFrame, fraction, unusedMicros, droppedTime); l--; )
		{
			if( !tickLogic(microsPerFrame) )
			{
				m_isRunning = false;
				break;
			}
		}
		m_logicTimer.updateBy(droppedTime);

		uint64_t fullUpdateTime = m_logicTimer.getLastRealTimeMicros() + unusedMicros - m_renderTimer.getLastRealTimeMicros();
		tickRender(fullUpdateTime);
		m_luaSystem.update();
		m_framerateTimer.update();
		return m_isRunning;
	}



	bool HedgehogGame::tickLogic(uint64_t updateTime)
	{
		bool continueRunning = true;
		m_logicTimer.updateBy(updateTime);

		m_inputSystem.update(m_logicTimer);
		auto& evs = m_inputSystem.getEvents();
		for( auto& e : evs )
		{
			for( auto& f : m_messageHandlers )
			{
				f(e);
			}
		}

		m_animationSystem.update(m_logicTimer.getDeltaMicros());

		auto lua = m_luaSystem.getStack();
		lua.pull("game_tick", 0);
		if( lua.isFunction() )
		{
			auto called = lua.call(CustomType{this, CLASS(HedgehogGame)}, &continueRunning);
			if( !called )
			{
				DEBUG_INFO(lua.toString());
				lua.pop();
				continueRunning = called;
			}
		}

		return continueRunning;
	}



	void HedgehogGame::tickRender(uint64_t updateTime)
	{
		m_renderTimer.updateBy(updateTime);

		m_graphicsSystem.begin();

		auto lua = m_luaSystem.getStack();
		lua.pull("game_render", 0);
		if( lua.isFunction() )
		{
			lua.call(CustomType{this, CLASS(HedgehogGame)});
		}

		static Transform framerateTf;
		framerateTf.position.set(0.5f*m_window->getSizeX() - 170, 0.5f*m_window->getSizeY() - 10);
		framerateTf.scale.set(0.5f, 0.5f);
		m_graphicsSystem.setOrthographicProjection();
		m_graphicsSystem.clearCamera();
		m_graphicsSystem.drawText(m_defaultFont, "ms per frame: " + std::to_string(Time::microsToMilis(m_framerateTimer.getDeltaMicros())), framerateTf, Color(0, 0, 0), 0, false);

		m_graphicsSystem.present();
	}
}
