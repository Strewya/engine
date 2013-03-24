//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "GameContext.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Core/Action/Action.h"
	/*** end headers ***/

namespace Core
{
	GameContext::GameContext(GameContextEvent onCreate, const ServiceLocator& services, const ResourceLocator& resources)
		: services(services), _onCreate(onCreate), _onActivate(nullptr), _onDeactivate(nullptr), _onDestroy(nullptr), _onUpdate(nullptr)
	{
		Action::BindActionUpdater(actionMaster);

		this->resources.Register(_textureCache);
		this->resources.Register(_spritesheetCache);
		//...

		_textureCache.setReferences(resources, services);
		_spritesheetCache.setReferences(resources, services);
		//...

		if(_onCreate)
		{
			_onCreate(*this);
		}
	}

	GameContext::~GameContext()
	{
		if(_onDestroy)
		{
			_onDestroy(*this);
		}
	}

	void GameContext::Activate()
	{
		Action::BindActionUpdater(actionMaster);

		if(_onActivate)
		{
			_onActivate(*this);
		}
	}

	void GameContext::Deactivate()
	{
		if(_onDeactivate)
		{
			_onDeactivate(*this);
		}
	}

	bool GameContext::Update()
	{
		if(_onUpdate)
		{
			_onUpdate(*this);
		}

		while(timer.TimeToUpdate(gUpdateInterval))
		{
			actionMaster.Update(gUpdateInterval, *this);
		}
		return true;
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