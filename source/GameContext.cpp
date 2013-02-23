//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "GameContext.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Core
{
	GameContext::GameContext(const ServiceLocator& services)
		: _services(services)
	{}

	bool GameContext::Update()
	{
		if(_timer.TimeToUpdate(gUpdateInterval))
		{
		}
		return true;
	}
}