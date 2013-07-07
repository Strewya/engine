#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common header ***/
#include "Engine/Defines.h"
#include <functional>
	/*** extra headers if needed ***/
	/*** end header inclusion ***/

namespace Core
{
	class GameContext;
}

//typedef bool(*GameContextEvent)(Core::GameContext& context);

typedef std::function<bool(Core::GameContext&)> GameContextEvent;