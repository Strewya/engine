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
#include <Modules/Graphics/Service/RendererFactory.h>
#include <Modules/Graphics/Service/SpritesheetCache.h>
#include <Modules/Input/Service/InputEngine.h>
#include <Modules/Physics/Service/PhysicsEngine.h>
#include <Modules/Scripting/Service/LuaEngine.h>
	/*** end header inclusion ***/

namespace Win32
{
	class Window;
}

namespace Core
{
	class Engine;

	class Engine
	{
	public:
		
		Engine(Win32::Window& window);
		~Engine();
		void loop();
		void shutdown();

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
		Physics::Engine m_physicsEngine;
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
