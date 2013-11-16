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

	protected:
		void registerActions();
		void registerCallbacks();
		void createEntities();

		//activation sequence
		void onActivate();

		//update sequence, optional as most of this is done by actions
		//void input(uint32_t dt);
		//void logic(uint32_t dt);

		//deactivate sequence
		void onDeactivate();

		//destroy sequence
		void destroyEntities();

	private:
		
		void setupLeftPaddle(Core::Entity& paddle);
		void setupRightPaddle(Core::Entity& paddle);
		void setupBall(Core::Entity& ball);
		void bindPaddleToField(Core::Entity& paddle, Core::Entity& field);
	};
}