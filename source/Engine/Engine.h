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
#include "Subsystems/Graphics/TextureCache.h"
#include "Subsystems/Input/InputEngine.h"
#include "Subsystems/Script/LuaEngine.h"
	/*** end header inclusion ***/

namespace Win32
{
	class AbstractWindow;
}

namespace Core
{
	class Engine
	{
	private:
		Win32::AbstractWindow& _window;
		Graphics::RendererFactory _rendererFactory;
		//Audio::AudioFactory _audioFactory;
		Script::Engine _scriptEngine;
		Input::Engine _inputEngine;
		//Network::CommEngine _commEngine;
		ServiceLocator _services;

		Graphics::TextureCache _textureCache;

		ResourceLocator _resources;

		std::unordered_map<String, std::unique_ptr<GameContext>> _gameContexts;
		GameContext* _activeContext;
	
	public:
		Engine(Win32::AbstractWindow& window);
		void Loop();
		void Shutdown();

		//GameContext& CreateContext(const char* name, GameContextEvent onCreate);
		//GameContext& CreateContext(const String& name, GameContextEvent onCreate);
		GameContext& getContext(const char* name, GameContextEvent onCreate = nullptr);
		GameContext& getContext(const String& name, GameContextEvent onCreate = nullptr);
		bool PushContext(const char* name);
		bool PushContext(const String& name);
	};
}