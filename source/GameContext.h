#ifndef CORE_GAME_CONTEXT_H_
#define CORE_GAME_CONTEXT_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
#include "ServiceLocator.h"
#include "Core/Action/ActionUpdater.h"
#include "Util/Clock.h"
	/*** end header inclusion ***/

namespace Core
{
	class Engine;

	class GameContext
	{
	private:
		ServiceLocator _services;
		ActionUpdater _actionMaster;
		Util::Timer _timer;
		Engine& _engine;
	
	public:
		GameContext(Engine& engine, const ServiceLocator& services);
		void Activate();
		void Deactivate();
		bool Update();
	};
}

#endif //CORE_GAME_CONTEXT_H_