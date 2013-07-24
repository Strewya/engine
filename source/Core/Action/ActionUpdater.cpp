//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Core/Action/ActionUpdater.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Engine/GameContext.h"
	/*** end headers ***/

namespace Core
{
	void ActionUpdater::update(float dt, GameContext& context)
	{
		for(auto& action : _priorityQueue)
		{
			
			action->update(dt, context);
		}
	}

	void ActionUpdater::addAction(ActionRef action)
	{
		return _priorityQueue.emplace_back(&action);
	}
}