#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common header ***/
#include <Engine/Defines.h>
#include <Engine/FwdDecl.h>
	/*** extra headers if needed ***/
	/*** end header inclusion ***/

namespace Pong
{
	bool createPaddle(Core::GameContext& context, Core::Entity& paddle);
	bool createBall(Core::GameContext& context, Core::Entity& ball);
	bool createField(Core::GameContext& context, Core::Entity& goal);
}