//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Core/Action/ActionUpdater.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Engine/GameContext.h"
	/*** end headers ***/

namespace Core
{
	ActionUpdater::ActionUpdater()
	{}
	
	void ActionUpdater::update(float dt, GameContext& context)
	{
		for(auto& pair : _priorityQueue)
		{
			auto& action = *pair.second;
			action.update(dt, context);
		}
	}

	bool ActionUpdater::addAction(std::unique_ptr<Action>& action)
	{
		if(_priorityQueue.find(action->getPriority()) == _priorityQueue.end())
		{
			return _priorityQueue.emplace(std::make_pair(action->getPriority(), std::move(action))).second;
		}
		return false;
	}
}