//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Core/Action/ActionRegistry.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Core/Action/Action.h"
	/*** end headers ***/

namespace Core
{
	ActionRef ActionRegistry::addAction(ActionUptr action)
	{
		return *m_actions.emplace(std::make_pair(action->uid, std::move(action))).first->second;
	}

	ActionRef ActionRegistry::getAction(InstanceID actionId)
	{
		//i could search here to find if the action is there and throw an exception straight away,
		//but this will fail on the line where the thing is used, so it'll be easier to debug.
		return *m_actions.find(actionId)->second;
	}

	auto ActionRegistry::begin() -> ActionStorage_t::iterator
	{
		return m_actions.begin();
	}

	auto ActionRegistry::end() -> ActionStorage_t::iterator
	{
		return m_actions.end();
	}
}