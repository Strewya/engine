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
	void ActionUpdater::update()
	{
		for(auto& action : m_priorityQueue)
		{
			action->update();
		}
	}

	void ActionUpdater::addAction(ActionRef action)
	{
		return m_priorityQueue.emplace_back(&action);
	}
}