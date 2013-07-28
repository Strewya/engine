	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Games/Pong/EntityConstructors.h"
	/*** extra headers ***/
#include "Engine/GameContext.h"
#include "Games/Pong/Components.h"
#include "Subsystems/Graphics/IRenderer.h"
	/*** end headers ***/

namespace Pong
{
	bool createPaddle(Core::GameContext& context, Core::Entity& paddle)
	{
		auto screen = context.services.getGraphics().getScreenSize();
		auto& pos = paddle.insert(Core::Position2d::create());
		

		return false;
	}

	bool createBall(Core::GameContext& context, Core::Entity& ball)
	{
		auto screen = context.services.getGraphics().getScreenSize();
		auto& pos = ball.insert(Core::Position2d::create());
		pos.value.Assign(screen.x/2, screen.y/2);
		
		return false;
	}
}