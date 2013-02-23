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
#include "Util/Clock.h"
	/*** end header inclusion ***/

namespace Core
{
	class GameContext
	{
	private:
		ServiceLocator _services;
		Util::Timer _timer;
	public:
		GameContext(const ServiceLocator& services);

		bool Update();
	};
}

#endif //CORE_GAME_CONTEXT_H_