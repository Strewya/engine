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

	void GameContext::update()
	{
		//get the amount of time that passed
		float delta = gUpdateInterval;
		//while the time passed is an increment of the default time step
		while(delta >= gUpdateInterval)
		{
			delta -= gUpdateInterval;
		//	input
			input();
		//	logic
			logic();
		}
		//after the logic has been updated, render
		render();
	}

	void GameContext::destroy()
	{}

	void GameContext::input()
	{
		//here i could pass the delta time to collect input, store them locally, and only handle those that happened within the time frame i am updating
		services.getInput().Update();
		onInput();
	}

	void GameContext::logic()
	{
		actionQueue.update();
		onLogic(gUpdateInterval);
	}

	void GameContext::render()
	{
		if(renderAction != nullptr)
		{
			renderAction->update();
		}
	}
	

	void GameContext::onInput()
	{}

	void GameContext::onLogic(float dt)
	{}
}
