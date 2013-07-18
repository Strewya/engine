//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Core/Action/Action.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/


namespace Core
{
	Action::Action()
	{}

	bool Action::registerEntity(InstanceID id)
	{
		return _entities.insert(id).second;
	}

	bool Action::unregisterEntity(InstanceID id)
	{
		return _entities.erase(id) > 0;
	}
}