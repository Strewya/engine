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
#include "Services/Graphics/b2DebugDraw.h"
#include "Util/Timer.h"
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
		void onUpdate(float dt);

		void setupLeftPaddle(Core::Entity& paddle);
		void setupRightPaddle(Core::Entity& paddle);
		void setupBall(Core::Entity& ball);
		void setupTopWall(Core::Entity& wall);
		void setupBottomWall(Core::Entity& wall);
		void setupLeftGoal(Core::Entity& wall);
		void setupRightGoal(Core::Entity& wall);

	private:
		Graphics::b2DebugDraw debug;
	};
}