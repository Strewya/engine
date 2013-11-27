#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include <Engine/Defines.h>
#include <memory>
#include <unordered_map>
	/*** extra headers if needed (alphabetically ordered) ***/
#include <Engine/GameContext.h>
#include <Engine/ResourceLocator.h>
#include <Engine/ServiceLocator.h>
#include <Services/Graphics/RendererFactory.h>
#include <Services/Graphics/SpritesheetCache.h>
#include <Services/Input/InputEngine.h>
#include <Services/Script/LuaEngine.h>
	/*** end header inclusion ***/

namespace Win32
{
	class Window;
}

namespace Core
{
	class Engine;

	typedef std::function<void(Engine&)> GameInitLogic;

	class Engine
	{
	public:
		
		Engine(Win32::Window& window);
		~Engine();
		void loop();
		void shutdown();

		/**
			Expects any callable type which receives a non-const reference to Engine class.
			The callable object must initialize all contexts that the game will use and register them in the engine.
			All resource loading can be done here to make use of caching.
			At the end of the function, one context must be pushed to be the active context.
		*/
		void initializeGame(GameInitLogic init);
		
		GameContext& registerContext(GameContext* context);
		GameContext& registerContext(std::unique_ptr<GameContext> context);
		template<typename T> GameContext& registerContext();
		GameContext& getContext(ContextType id);
		bool setActiveContext(ContextType id);
		bool setActiveContext(GameContext& context);

		ServiceLocator& getServices();
		ResourceLocator& getResources();

	private:
		Win32::Window& m_window;
		Graphics::RendererFactory m_rendererFactory;
		//Audio::AudioFactory _audioFactory;
		Script::Engine m_scriptEngine;
		Input::Engine m_inputEngine;
		//Network::CommEngine _commEngine;
		ServiceLocator m_services;

		
		Graphics::SpritesheetCache m_spritesheets;
		ResourceLocator m_resources;

		typedef std::unordered_map<uint32_t, std::unique_ptr<GameContext>> ContextStorage_t;

		ContextStorage_t m_gameContexts;
		GameContext* m_activeContext;

		Util::Time m_mainTimer;
		Util::Time m_renderTimer;
	};



	template<typename T> GameContext& Engine::registerContext()
	{
		return registerContext(std::unique_ptr<GameContext>(new T(m_services, m_resources)));
	}
}
