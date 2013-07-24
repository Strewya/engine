//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Core/Action/ActionIndex.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Core/Action/Action.h"
	/*** end headers ***/

namespace Core
{
	ActionRef ActionIndex::addActionToIndex(ActionUptr action)
	{
		return *_actions.emplace(std::make_pair(action->uid, std::move(action))).first->second;
	}

	ActionRef ActionIndex::getActionFromIndex(InstanceID actionId)
	{
		//i could search here to find if the action is there and throw an exception straight away,
		//but this will fail on the line where the thing is used, so it'll be easier to debug.
		return *_actions.find(actionId)->second;
	}
}