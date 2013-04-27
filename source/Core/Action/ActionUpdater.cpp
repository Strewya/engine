//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Core/Action/ActionUpdater.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Engine/GameContext.h"
#include "Core/Action/Action.h"
	/*** end headers ***/

namespace Core
{
	void ActionUpdater::Update(float dt, GameContext& context)
	{
		for(ActionLogic fn : _actionQueue)
		{
			auto& list = _activeActions[fn];
			if(!list.empty())
			{
				fn(dt, context, list);
			}
		}
		PurgeActions();
	}

	bool ActionUpdater::EnqueueActionLogic(ActionLogic logic)
	{
		auto it = std::find(_actionQueue.begin(), _actionQueue.end(), logic);
		if(it != _actionQueue.end())
		{
			return false;
		}
		_actionQueue.push_back(logic);
		return true;
	}

	bool ActionUpdater::ActivateAction(ActionLogic logic, Action* action)
	{
		auto& allActions = _activeActions[logic];
		auto it = allActions.find(action);
		if(it == allActions.end())
		{
			allActions.insert(action);
			return true;
		}
		return false;
	}

	bool ActionUpdater::DeactivateAction(ActionLogic logic, Action* action)
	{
		auto& allActions = _activeActions[logic];
		auto it = allActions.find(action);
		if(it != allActions.end())
		{
			_actionsForDeletion.push_back(std::make_pair(logic, action));
			return true;
		}
		return false;
	}

	void ActionUpdater::PurgeActions()
	{
		for(auto& pair : _actionsForDeletion)
		{
			auto& allActions = _activeActions[pair.first];
			auto it = allActions.find(pair.second);
			if(it != allActions.end())
			{
				allActions.erase(it);
			}
		}
		_actionsForDeletion.clear();
	}
}