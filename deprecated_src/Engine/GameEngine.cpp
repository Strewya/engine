//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include <stdafx.h>
	/*** personal header ***/
#include <Engine/GameEngine.h>
	/*** C++ headers ***/
	/*** extra headers ***/
#include <Modules/Graphics/Service/IRenderer.h>
#include <Win32/Window.h>
	/*** end headers ***/

namespace Core
{
	Engine::Engine(Win32::Window& window)
		: m_window(window), m_rendererFactory(window), m_inputEngine(window), m_activeContext(nullptr)
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
		std::string renderer = "dx";
		/*
			initialize the subsystems:
			* display - done
			* audio
			* input - done (in initialization list)
			* script - done (no special ctor)
			* communications
			* persistence
		*/
		if(!m_rendererFactory.initInterface(renderer))
		{
			throw std::logic_error("Unable to initialize renderer");
		}
		//_rendererFactory.getInterface()->setScreenSize(_window.getSizeX(), _window.getSizeY());

		/*
			register all of the initialized subsystems
			* display - done
			* audio
			* input - done
			* script - done
			* comms
			* persistence
		*/
		m_services.bind(*this);
		m_services.bind(m_rendererFactory.getInterface());
		m_services.bind(m_scriptEngine);
		m_services.bind(m_inputEngine);
		m_services.bind(m_physicsEngine);

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
		for(auto it = m_gameContexts.begin(); it != m_gameContexts.end(); ++it)
		{
			it->second->destroy();
		}
		m_gameContexts.clear();
		m_spritesheets.clear();
		m_rendererFactory.destroyInterface();
	}

	void Engine::loop()
	{
		if(!m_activeContext)
		{
			m_window.showMessagebox("Error", "No game context active, exiting!");
			shutdown();
			return;
		}

		float ratio = 0;
		uint64_t unusedTime = 0;
		uint64_t updateTime = m_activeContext->m_updateTime;
		uint32_t updateCount = m_mainTimer.getFixedStepUpdateCount(updateTime, ratio, unusedTime);

		while(updateCount-- > 0)
		{
			m_mainTimer.updateBy(updateTime, m_activeContext->getTimeScale());
			m_activeContext->update();
		}

		uint64_t fullUpdateTime = m_mainTimer.getLastRealTimeMicros() + unusedTime - m_renderTimer.getLastRealTimeMicros();
		m_renderTimer.updateBy(fullUpdateTime, m_activeContext->getTimeScale());
        m_activeContext->render(fullUpdateTime);
	}

	void Engine::shutdown()
	{
		m_window.shutdown();
	}

	ServiceLocator& Engine::getServices()
	{
		return m_services;
	}

	ResourceLocator& Engine::getResources()
	{
		return m_resources;
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
		it->second->init();
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
