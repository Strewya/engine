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
		destroy();
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
				logic(timer.m_targetTime);
			}
			render();
		}
	}

	void GameContext::destroy()
	{
	}

	void GameContext::input(uint32_t dt)
	{
		//here i could pass the delta time to collect input, store them locally, and only handle those that happened within the time frame i am updating
		services.getInput().update();
		onInput(dt);
	}

	void GameContext::logic(uint32_t dt)
	{
		actionQueue.update();
		onLogic(dt);
	}

	void GameContext::render()
	{
		if(renderAction != nullptr)
		{
			renderAction->update();
		}
	}
	

	void GameContext::onInput(uint32_t dt)
	{}

	void GameContext::onLogic(uint32_t dt)
	{}
}
