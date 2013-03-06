#ifndef CORE_ACTION_ACTION_LOGIC_H_
#define CORE_ACTION_ACTION_LOGIC_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
#include <set>
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Core
{
	class Action;
	class GameContext;
}

typedef void(*ActionLogic)(double dt, Core::GameContext& context, std::set<Core::Action*>& actions);

#endif //CORE_ACTION_ACTION_LOGIC_H_