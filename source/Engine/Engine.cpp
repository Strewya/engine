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
		: _window(window), _rendererFactory(window), _activeContext(nullptr)
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
		if(!_rendererFactory.InitInterface(renderer))
		{
			throw;
		}
		_rendererFactory.getInterface()->setScreenSize(_window.getSizeX(), _window.getSizeY());

		_window.setEventQueue(_inputEngine.getEventQueue());
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
		if(_activeContext->update())
		{
			_inputEngine.PurgeEvents();
		}
	}

	void Engine::shutdown()
	{
		_window.Shutdown();
	}

	ServiceLocator& Engine::getServices()
	{
		return _services;
	}

	ResourceLocator& Engine::getResources()
	{
		return _resources;
	}

	void Engine::initializeGame(GameInitLogic init)
	{
		init(*this);
	}
	
	GameContext& Engine::registerContext(GameContext* context)
	{
		return registerContext(std::unique_ptr<GameContext>(context));
	}

	GameContext& Engine::registerContext(std::unique_ptr<GameContext> context)
	{
		auto it = _gameContexts.find(context->Type);
		if(it == _gameContexts.end())
		{
			it = _gameContexts.emplace(context->Type, std::move(context)).first;
		}
		else
		{
			it->second.swap(context);
		}
		_mainClock.RegisterTimer(it->second->timer);
		return *it->second;
	}

	GameContext& Engine::getContext(ContextType id)
	{
		auto it = _gameContexts.find(id);
		if(it == _gameContexts.end())
		{
			_window.showMessagebox("Error", "Attempt to get invalid context ID, shutting down.");
			shutdown();
			throw std::exception();
		}
		return *it->second;
	}

	bool Engine::setActiveContext(ContextType id)
	{
		auto it = _gameContexts.find(id);
		if(it != _gameContexts.end())
		{
			return setActiveContext(*it->second);
		}
		return false;
	}
	
	bool Engine::setActiveContext(GameContext& context)
	{
		if(_activeContext != &context)
		{
			if(_activeContext != nullptr)
			{
				_activeContext->deactivate();
			}
			_activeContext = &context;
			_activeContext->activate();
			return true;
		}
		return false;
	}
}
