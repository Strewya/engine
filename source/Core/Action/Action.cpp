//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Core/Action/Action.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Engine/GameContext.h"
	/*** end headers ***/


namespace Core
{
	Action::Action(InstanceID type, GameContext& context)
		: uid(type), m_context(context)
	{
		context.timer.RegisterTimer(m_timer);
	}

	bool Action::update()
	{
		while(!m_timer.isPaused && m_timer.hasUpdatePeriodElapsed())
		{
			return onUpdate(m_timer.getUpdatePeriod());
		}
		return false;
	}

	bool Action::registerEntity(InstanceID id)
	{
		return m_entities.insert(id).second;
	}

	bool Action::unregisterEntity(InstanceID id)
	{
		return m_entities.erase(id) > 0;
	}
}