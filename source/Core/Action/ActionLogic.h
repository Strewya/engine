#ifndef CORE_ACTION_ACTIONLOGIC_H_
#define CORE_ACTION_ACTIONLOGIC_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <set>
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Core
{
	class Action;
	class GameContext;
}

typedef void(*ActionLogic)(double dt, Core::GameContext& context, std::set<Core::Action*>& actions);

#endif //CORE_ACTION_ACTIONLOGIC_H_