#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <memory>
#include <unordered_map>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Engine/GameContext.h"
#include "Engine/ResourceLocator.h"
#include "Engine/ServiceLocator.h"
#include "Subsystems/Graphics/RendererFactory.h"
#include "Subsystems/Graphics/ITextureCache.h"
#include "Subsystems/Input/InputEngine.h"
#include "Subsystems/Script/LuaEngine.h"
#include "Util/Clock.h"
	/*** end header inclusion ***/

namespace Win32
{
	class Window;
}

namespace Core
{
	class Engine
	{
	public:
		typedef std::function<void(Engine&)> GameInitLogic;
		Engine(Win32::Window& window);
		~Engine();
		void loop();
		void shutdown();

		void initializeGame(GameInitLogic init);
		GameContext& getContext(const char* name);
		GameContext& getContext(const String& name);
		bool pushContext(const char* name);
		bool pushContext(const String& name);
	
	private:
		Win32::Window& _window;
		Graphics::RendererFactory _rendererFactory;
		//Audio::AudioFactory _audioFactory;
		Script::Engine _scriptEngine;
		Input::Engine _inputEngine;
		//Network::CommEngine _commEngine;
		ServiceLocator _services;

		ResourceLocator _resources;
		Graphics::SpritesheetCache _spritesheets;

		std::unordered_map<String, std::unique_ptr<GameContext>> _gameContexts;
		GameContext* _activeContext; 
		
		Util::Clock _mainClock;
	};
}