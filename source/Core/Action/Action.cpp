//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Core/Action/Action.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/


namespace Core
{
	Action::Action(InstanceID type)
		: uid(type)
	{}

	bool Action::update(float dt, GameContext& context)
	{
		if(!_timer.isPaused && _timer.hasUpdatePeriodElapsed())
		{
			return onUpdate(dt, context);
		}
		return false;
	}

	bool Action::registerEntity(InstanceID id)
	{
		return _entities.insert(id).second;
	}

	bool Action::unregisterEntity(InstanceID id)
	{
		return _entities.erase(id) > 0;
	}
}