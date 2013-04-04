#ifndef CORE_GAMECONTEXTEVENT_H_
#define CORE_GAMECONTEXTEVENT_H_
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

#endif //CORE_GAMECONTEXTEVENT_H_