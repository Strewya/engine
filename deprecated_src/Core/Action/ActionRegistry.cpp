//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include <stdafx.h>
	/*** personal header ***/
#include <Core/Action/ActionRegistry.h>
	/*** C++ headers ***/
	/*** extra headers ***/
#include <Core/Action/Action.h>
	/*** end headers ***/

namespace Core
{
	ActionRef ActionRegistry::addAction(ActionUptr action)
	{
		assert(action->getFamily() == ActionFamily::Composite || m_actions.count(action->getUID()) == 0);
		ActionRef ref = *action;
		m_actions.emplace(std::make_pair(action->getUID(), std::move(action)));
		return ref;
	}
}