#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common header ***/
#include "Engine/Defines.h"
	/*** extra headers if needed ***/
	/*** end header inclusion ***/

namespace Core
{
	class GameContext;
}

typedef bool(*GameContextEvent)(Core::GameContext& context);