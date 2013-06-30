//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Engine/GameContext.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Core/Action/Action.h"
	/*** end headers ***/

namespace Core
{
	GameContext::GameContext(GameContextEvent onCreate, ServiceLocator& services, ResourceLocator& resources)
		: _onCreate(onCreate), _onActivate(nullptr), _onDeactivate(nullptr), _onDestroy(nullptr), _onUpdate(nullptr),
		timer(), physicsWorld(b2Vec2(0,0)), entityFactory(*this), services(services), resources(resources)
	{
		Action::BindActionUpdater(actionMaster);
	}

	GameContext::~GameContext()
	{
		Destroy();
	}

	void GameContext::Create()
	{
		if(_onCreate)
		{
			_onCreate(*this);
		}
	}

	void GameContext::Destroy()
	{
		if(_onDestroy)
		{
			_onDestroy(*this);
		}
	}

	void GameContext::Activate()
	{
		Action::BindActionUpdater(actionMaster);
		timer.isPaused = false;

		if(_onActivate)
		{
			_onActivate(*this);
		}
	}

	void GameContext::Deactivate()
	{
		timer.isPaused = true;

		if(_onDeactivate)
		{
			_onDeactivate(*this);
		}
	}

	bool GameContext::Update()
	{
		if(!timer.isPaused)
		{
			if(_onUpdate)
			{
				_onUpdate(*this);
			}

			actionMaster.Update(gUpdateInterval, *this);
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