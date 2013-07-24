//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Engine/Engine.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Subsystems/Graphics/IRenderer.h"
#include "Win32/Window.h"
	/*** end headers ***/

namespace Core
{
	Engine::Engine(Win32::Window& window)
		: _window(window), _rendererFactory(window), _activeContext(nullptr), _inputEngine(window)
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
			register the resource caches
			* textures - done
			* spritesheets
			* meshes
			* sounds
			* scripts
			* TODO: strings
		*/
		_resources.Register(_services.getGraphics().getTextureCache());
		_spritesheets.setTextureCache(_resources.getTextureCache());
		_resources.Register(_spritesheets);

		/*
			load the game configuration script and execute it
			* the script should create the game contexts that define the game's execution flow

			push the main context on the context stack

			~~end of initialization, if everything passes and no exceptions are thrown, the configuration is OK
		*/
	}

	Engine::~Engine()
	{
		_gameContexts.clear();
		_spritesheets.clear();
	}

	void Engine::loop()
	{
		if(!_activeContext)
		{
			_window.showMessagebox("Error", "No game context active, exiting!");
			shutdown();
			return;
		}

		_mainClock.AdvanceTime();

		//update input
		_inputEngine.Update();

		//update logic and draw entities
		//only one context can be active
		if(_activeContext->Update())
		{
			_inputEngine.PurgeEvents();
		}
	}

	void Engine::shutdown()
	{
		_window.Shutdown();
	}

	void Engine::initializeGame(GameInitLogic init)
	{
		init(*this);
	}
	
	GameContext& Engine::getContext(const char* name)
	{
		auto it = _gameContexts.find(name);
		if(it == _gameContexts.end())
		{
			it = _gameContexts.emplace(name, std::unique_ptr<GameContext>(new GameContext(_services, _resources))).first;
			_mainClock.RegisterTimer(it->second->timer);
		}
		return *it->second;
	}

	GameContext& Engine::getContext(const String& name)
	{
		return getContext(name.c_str());
	}

	bool Engine::pushContext(const char* name)
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

	bool Engine::pushContext(const String& name)
	{
		return pushContext(name.c_str());
	}
}