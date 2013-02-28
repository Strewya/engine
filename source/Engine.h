#ifndef CORE_ENGINE_H_
#define CORE_ENGINE_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
#include <list>
#include <memory>
#include <unordered_map>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "GameContext.h"
#include "ResourceLocator.h"
#include "ServiceLocator.h"
#include "Subsystems/Graphics/RendererFactory.h"
#include "Subsystems/Graphics/TextureCache.h"
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
		const Win32::AbstractWindow& _window;
		Graphics::RendererFactory _rendererFactory;
		//Audio::AudioFactory _audioFactory;
		Script::LuaEngine _scriptEngine;
		//Input::InputEngine _inputEngine;
		//Network::CommEngine _commEngine;
		ServiceLocator _services;

		Graphics::TextureCache _textureCache;

		ResourceLocator _resources;

		std::unordered_map<String, std::unique_ptr<GameContext>> _gameContexts;
		GameContext* _activeContext;
	
	public:
		Engine(const Win32::AbstractWindow& window);
		void Loop();
		void Shutdown();

		//GameContext& CreateContext(const char* name);
		//GameContext& CreateContext(const String& name);
		GameContext& getContext(const char* name);
		GameContext& getContext(const String& name);
		bool PushContext(const char* name);
		bool PushContext(const String& name);
	};
}

#endif //CORE_ENGINE_H_