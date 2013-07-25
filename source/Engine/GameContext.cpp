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
	GameContext::GameContext(ServiceLocator& services, ResourceLocator& resources)
		: _onActivate(nullptr), _onDeactivate(nullptr), _onDestroy(nullptr), _onUpdate(nullptr),
		physicsWorld(b2Vec2(0,0)), entityFactory(*this), services(services), resources(resources)
	{
	}

	GameContext::~GameContext()
	{
		destroy();
	}

	void GameContext::destroy()
	{
		if(_onDestroy)
		{
			_onDestroy(*this);
		}
	}

	void GameContext::activate()
	{
		timer.isPaused = false;

		if(_onActivate)
		{
			_onActivate(*this);
		}
	}

	void GameContext::deactivate()
	{
		timer.isPaused = true;

		if(_onDeactivate)
		{
			_onDeactivate(*this);
		}
	}

	bool GameContext::update()
	{
		if(!timer.isPaused)
		{
			if(_onUpdate)
			{
				_onUpdate(*this);
			}

			actionQueue.update(gUpdateInterval, *this);
			return true;
		}
		return false;
	}

	void GameContext::setContextEventLogic(EventType type, GameContextEvent function)
	{
		switch(type)
		{
		case OnActivate:
			_onActivate = function;
			break;
		case OnDeactivate:
			_onDeactivate = function;
			break;
		case OnUpdate:
			_onUpdate = function;
			break;
		case OnDestroy:
			_onDestroy = function;
			break;
		}
	}
}