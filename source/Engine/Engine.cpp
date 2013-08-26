//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Engine/Engine.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Services/Graphics/IRenderer.h"
#include "Win32/Window.h"
	/*** end headers ***/

namespace Core
{
	Engine::Engine(Win32::Window& window)
		: m_window(window), m_rendererFactory(window), m_activeContext(nullptr)
	{
		SYSTEMTIME st;
		GetSystemTime(&st);
		Util::Logger& log = Util::GetDefaultLogger();

		log << log.endl;
		log << "*********************************************" << log.endl;
		log	<< "* New program start @ ";
		log << st.wHour << ":" << st.wMinute << ":" << st.wSecond << " on " << st.wDay << "." << st.wMonth << "." << st.wYear << ".  ";
		log << "Application: " << m_window.getTitle() << log.endl;
		log << "*********************************************" << log.endl << log.endl;

		/*	read the config file for engine configuration, which needs to include the following:
			* fullscreen status
			* resolution (width and height)
			* display engine (directx by default)
			* audio engine (fmod by default)
		*/
		m_window.setCursorShow(true);
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
		if(!m_rendererFactory.InitInterface(renderer))
		{
			throw std::logic_error("Unable to initialize renderer");
		}
		//_rendererFactory.getInterface()->setScreenSize(_window.getSizeX(), _window.getSizeY());

		m_window.setEventQueue(m_inputEngine.getEventQueue());
		/*
			register all of the initialized subsystems
			* display - done
			* audio
			* input - done
			* script - done
			* comms
			* persistence
		*/
		m_services.Register(this);
		m_services.Register(m_rendererFactory.getInterface());
		m_services.Register(&m_scriptEngine);
		m_services.Register(&m_inputEngine);

		/*
			register the resource caches
			* textures - done
			* spritesheets
			* meshes
			* sounds
			* scripts
			* TODO: strings
		*/
		//_resources.Register(_services.getGraphics().getTextureCache());
		m_spritesheets.setTextureCache(m_resources.getTextureCache());
		m_resources.Register(m_spritesheets);

		/*
			load the game configuration script and execute it
			* the script should create the game contexts that define the game's execution flow

			push the main context on the context stack

			~~end of initialization, if everything passes and no exceptions are thrown, the configuration is OK
		*/
	}

	Engine::~Engine()
	{
		m_gameContexts.clear();
		m_spritesheets.clear();
		m_rendererFactory.DestroyInterface();
	}

	void Engine::loop()
	{
		if(!m_activeContext)
		{
			m_window.showMessagebox("Error", "No game context active, exiting!");
			shutdown();
			return;
		}

		m_mainClock.AdvanceTime();
		
		//update logic and draw entities
		//only one context can be active at a time
		m_activeContext->update();
	}

	void Engine::shutdown()
	{
		m_window.Shutdown();
	}

	ServiceLocator& Engine::getServices()
	{
		return m_services;
	}

	ResourceLocator& Engine::getResources()
	{
		return m_resources;
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
		auto it = m_gameContexts.find(context->Type);
		if(it == m_gameContexts.end())
		{
			it = m_gameContexts.emplace(context->Type, std::move(context)).first;
		}
		else
		{
			it->second.swap(context);
		}
		m_mainClock.RegisterTimer(it->second->timer);
		return *it->second;
	}

	GameContext& Engine::getContext(ContextType id)
	{
		auto it = m_gameContexts.find(id);
		if(it == m_gameContexts.end())
		{
			m_window.showMessagebox("Error", "Attempt to get invalid context ID, shutting down.");
			shutdown();
			throw std::exception();
		}
		return *it->second;
	}

	bool Engine::setActiveContext(ContextType id)
	{
		auto it = m_gameContexts.find(id);
		if(it != m_gameContexts.end())
		{
			return setActiveContext(*it->second);
		}
		return false;
	}
	
	bool Engine::setActiveContext(GameContext& context)
	{
		if(m_activeContext != &context)
		{
			if(m_activeContext != nullptr)
			{
				m_activeContext->deactivate();
			}
			m_activeContext = &context;
			m_activeContext->activate();
			return true;
		}
		return false;
	}
}
