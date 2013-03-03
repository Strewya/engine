#ifndef CORE_GAMECONTEXT_EVENT_H_
#define CORE_GAMECONTEXT_EVENT_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common header ***/
#include "Defines.h"
	/*** extra headers if needed ***/
	/*** end header inclusion ***/

namespace Core
{
	class GameContext;
}

typedef bool(*GameContextEvent)(Core::GameContext& context);

#endif //CORE_GAMECONTEXT_EVENT_H_