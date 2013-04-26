#pragma once
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