#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common header ***/
#include "Engine/Defines.h"
#include "Engine/FwdDecl.h"
	/*** extra headers if needed ***/
#include "Engine/GameContextEvent.h"
	/*** end header inclusion ***/

namespace Pong
{
	bool CreatePaddle(Core::GameContext& context, Core::Entity& target);
	bool CreateBall(Core::GameContext& context, Core::Entity& target);

	bool ContextGameplayCreate(Core::GameContext& context);
}