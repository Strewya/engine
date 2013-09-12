//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Engine/GameContext.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Core/Action/Action.h"
#include "Engine/Engine.h"
#include "Services/Input/InputEngine.h"
	/*** end headers ***/

namespace Core
{
	GameContext::GameContext(ContextType type, ServiceLocator& services, ResourceLocator& resources)
		: Type(type),
		services(services), //for ease of access to services
		resources(resources), //for ease of access to resources
		m_timerId(INVALID_ID),
		m_timerExpired(false),
		entityFactory(*this), //not sure if this should be here or more globally, like the Engine object
		physicsWorld(b2Vec2(0,0)), //not sure if this should be here or in the Physics Action/Behavior/System
		b2ScalingFactor(1),
		activeEntities(NO_ID) //the currently active entities in the context
	{
		m_timerId = services.getClock().createAccumulator(services.getClock().getTimePerFrame(), [&](){m_timerExpired=true;});
	}

	GameContext::~GameContext()
	{
	}

	// event methods
	void GameContext::init()
	{
		registerActions();
		setupActionQueue();
		for(auto it = actionRegistry.begin(); it != actionRegistry.end(); ++it)
		{
			it->second->registerCallbacks();
		}
		registerCallbacks();
		createEntities();
	}

	void GameContext::activate()
	{
		services.getClock().getTimer(m_timerId).resume();
		onActivate();
	}

	void GameContext::update()
	{
		if(m_timerExpired)
		{
			auto& timer = services.getClock().getTimer(m_timerId);
			while(timer.m_currentTime >= timer.m_targetTime)
			{
				timer.m_currentTime -= timer.m_targetTime;

				input(timer.m_targetTime);
				//logic
				actionQueue.update();
				logic(timer.m_targetTime);
			}
			//render
			if(renderAction != nullptr)
			{
				renderAction->update();
			}
		}
	}

	void GameContext::deactivate()
	{
		services.getClock().getTimer(m_timerId).pause();
		onDeactivate();
	}

	void GameContext::destroy()
	{
		services.getClock().deleteTimer(m_timerId);
		destroyEntities();
	}




	void GameContext::input(uint32_t dt)
	{}

	void GameContext::logic(uint32_t dt)
	{}
		
	
}
