//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Engine.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Win32/AbstractWindow.h"
	/*** end headers ***/

namespace Core
{
	Engine::Engine(const Win32::AbstractWindow& window) : _window(window), _rendererFactory(window.getInstance(), window.getWindowHandle())
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
			* display
			* audio
			* input
			* script
			* communications
			* persistence
		*/
		_rendererFactory.InitInterface(renderer);
		
		_services.Register(this);
		_services.Register(_rendererFactory.getInterface());
		

		/*
			initialize the resource caches
			* textures
			* spritesheets
			* meshes
			* sounds
			* scripts
			* TODO: strings
		*/

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
		//if there can be only one active context
		_activeContext->Update();
		
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
		PostMessage(_window.getWindowHandle(), WM_DESTROY, 0, 0);
	}
	/*
	GameContext& Engine::CreateContext(const char* name)
	{
	}

	GameContext& Engine::CreateContext(const String& name)
	{
		return CreateContext(name.c_str());
	}
	*/
	GameContext& Engine::getContext(const char* name)
	{
		auto it = _gameContexts.find(name);
		if(it == _gameContexts.end())
		{
			it = _gameContexts.emplace(name, std::unique_ptr<GameContext>(new GameContext(*this, _services))).first;
		}
		return *it->second;
	}

	GameContext& Engine::getContext(const String& name)
	{
		return getContext(name.c_str());
	}

	bool Engine::PushContext(const char* name)
	{
		auto it = _gameContexts.find(name);
		if(it != _gameContexts.end() && it->second.get() != _activeContext)
		{
			_activeContext->Deactivate();
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