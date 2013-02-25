//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Core/Action/ActionUpdater.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Core/Entity/Entity.h"
	/*** end headers ***/

namespace Core
{
	void ActionUpdater::Update(float dt, ServiceLocator& services)
	{
		for(ActionLogic fn : _actionQueue)
		{
			fn(dt, services, _activeActions[fn]);
		}
	}

	bool ActionUpdater::EnqueueAction(ActionLogic logic)
	{
		auto it = std::find(_actionQueue.begin(), _actionQueue.end(), logic);
		if(it != _actionQueue.end())
		{
			return false;
		}
		_actionQueue.push_back(logic);
		return true;
	}

	bool ActionUpdater::ActivateEntity(ActionLogic logic, Entity* entity)
	{
		auto& entities = _activeActions[logic];
		auto it = entities.find(entity);
		if(it == entities.end())
		{
			entities.insert(entity);
			return true;
		}
		return false;
	}
}