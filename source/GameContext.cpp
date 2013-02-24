//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "GameContext.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Core/Action/Action.h"
	/*** end headers ***/

namespace Core
{
	GameContext::GameContext(Engine& engine, const ServiceLocator& services)
		: _engine(engine), _services(services)
	{}

	void GameContext::Activate()
	{
		Action::BindActionUpdater(_actionMaster);
	}

	void GameContext::Deactivate()
	{
	}

	bool GameContext::Update()
	{
		if(_timer.TimeToUpdate(gUpdateInterval))
		{
		}
		return true;
	}

}