#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common header ***/
#include "Engine/Defines.h"
#include "Engine/FwdDecl.h"
	/*** extra headers if needed ***/
#include "Engine/GameContext.h"
	/*** end header inclusion ***/

namespace Pong
{
	class Gameplay : public Core::GameContext
	{
	public:
		Gameplay(Core::ServiceLocator& services, Core::ResourceLocator& resources);

		void activate();
		void deactivate();
		void registerActions();
		void setupActionQueue();

		void setupLeftPaddle(Core::Entity& paddle);
		void setupRightPaddle(Core::Entity& paddle);
		void setupBall(Core::Entity& ball);
	};
}