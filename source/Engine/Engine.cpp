//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Engine/Engine.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Subsystems/Graphics/IRenderer.h"
#include "Win32/AbstractWindow.h"
	/*** end headers ***/

namespace Core
{
	Engine::Engine(Win32::AbstractWindow& window) 
		: _window(window), _rendererFactory(window.getInstance(), window.getWindowHandle()), _activeContext(nullptr), _inputEngine(window)
	{
		SYSTEMTIME st;
		GetSystemTime(&st);
		Util::Logger& log = Util::GetDefaultLogger();

		log << log.endl;
		log << "*********************************************" << log.endl;
		log	<< "* New program start @ ";
		log << st.wHour << ":" << st.wMinute << ":" << st.wSecond << " on " << st.wDay << "." << st.wMonth << "." << st.wYear << ".  ";
		log << "Application: " << _window.getTitle() << log.endl;
		log << "*********************************************" << log.endl << log.endl;

		/*	read the config file for engine configuration, which needs to include the following:
			* fullscreen status
			* resolution (width and height)
			* display engine (directx by default)
			* audio engine (fmod by default)
		*/
		String renderer = "dx";
		/*
			initialize the subsystems:
			* display - done
			* audio
			* input - done (in initialization list)
			* script - done (no special ctor)
			* communications
			* persistence
		*/
		_rendererFactory.InitInterface(renderer);
		_rendererFactory.getInterface()->setScreenSize(_window.getSizeX(), _window.getSizeY());
		/*
			register all of the initialized subsystems
			* display - done
			* audio
			* input - done
			* script - done
			* comms
			* persistence
		*/
		_services.Register(this);
		_services.Register(_rendererFactory.getInterface());
		_services.Register(&_scriptEngine);
		_services.Register(&_inputEngine);

		/*
			initialize the resource caches
			* textures - done
			* spritesheets
			* meshes
			* sounds
			* scripts
			* TODO: strings
		*/
		_textureCache.setReferences(_resources, _services);

		/*
			register the resource caches
			* textures - done
			* spritesheets
			* meshes
			* sounds
			* scripts
			* TODO: strings
		*/
		_resources.Register(_textureCache);

		/*
			load the game configuration script and execute it
			* the script should create the game contexts that define the game's execution flow

			push the main context on the context stack

			~~end of initialization, if everything passes and no exceptions are thrown, the configuration is OK
		*/
	}

	void Engine::Loop()
	{
		if(!_activeContext)
		{
			Shutdown();
		}

		//update input
		_inputEngine.Update();

		//update logic and draw entities
		//if there can be only one active context
		if(_activeContext->Update())
		{
			_inputEngine.PurgeEvents();
		}
		
		//if there are more than one active contexts
		/*
		for(auto context : _activeContexts)
		{
			if(context->Update())
			{
				break;
			}
		}
		*/

		
	}

	void Engine::Shutdown()
	{
		_window.Shutdown();
	}
	/*
	GameContext& Engine::CreateContext(const char* name, GameContextEvent onCreate)
	{
		auto it = _gameContexts.find(name);
		if(it == _gameContexts.end())
		{
			it = _gameContexts.emplace(name, std::unique_ptr<GameContext>(new GameContext(onCreate, *this, _services, _resources))).first;
		}
		return *it->second;
	}

	GameContext& Engine::CreateContext(const String& name, GameContextEvent onCreate)
	{
		return CreateContext(name.c_str(), onCreate);
	}
	*/
	GameContext& Engine::getContext(const char* name, GameContextEvent onCreate)
	{
		auto it = _gameContexts.find(name);
		if(it == _gameContexts.end())
		{
			it = _gameContexts.emplace(name, std::unique_ptr<GameContext>(new GameContext(onCreate, _services, _resources))).first;
			it->second->Create();
		}
		return *it->second;
	}

	GameContext& Engine::getContext(const String& name, GameContextEvent onCreate)
	{
		return getContext(name.c_str(), onCreate);
	}

	bool Engine::PushContext(const char* name)
	{
		auto it = _gameContexts.find(name);
		if(it != _gameContexts.end() && it->second.get() != _activeContext)
		{
			if(_activeContext != nullptr)
			{
				_activeContext->Deactivate();
			}
			_activeContext = it->second.get();
			_activeContext->Activate();
			return true;
		}
		return false;
	}

	bool Engine::PushContext(const String& name)
	{
		return PushContext(name.c_str());
	}
}