//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Engine/GameContext.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Core/Action/Action.h"
	/*** end headers ***/

namespace Core
{
	GameContext::GameContext(ContextType type, ServiceLocator& services, ResourceLocator& resources)
		: Type(type),
		entityFactory(*this), //not sure if this should be here or more globally, like the Engine object
		services(services), //for ease of access to services
		resources(resources), //for ease of access to resources
		physicsWorld(b2Vec2(0,0)), //not sure if this should be here or in the Physics Action/Behavior/System
		b2ScalingFactor(1),
		activeEntities(0) //the currently active entities in the context
	{}

	GameContext::~GameContext()
	{
		destroy();
	}

	void GameContext::destroy()
	{}

	bool GameContext::update()
	{
		if(!timer.isPaused)
		{
			onUpdate(gUpdateInterval);

			actionQueue.update();
			
			return true;
		}
		return false;
	}

	void GameContext::onUpdate(float dt)
	{}
}
