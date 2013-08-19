	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Games/Pong/EntityConstructors.h"
	/*** extra headers ***/
#include "Engine/GameContext.h"
#include "Games/Pong/Components.h"
#include "Services/Graphics/IRenderer.h"
	/*** end headers ***/

namespace Pong
{
	bool createPaddle(Core::GameContext& context, Core::Entity& paddle)
	{
		paddle.insert(Core::Position2d::create());


		return false;
	}

	bool createBall(Core::GameContext& context, Core::Entity& ball)
	{
		auto screen = Util::Vec2(800,600); //context.services.getGraphics().getScreenSize();
		auto& pos = ball.insert(Core::Position2d::create());
		pos.data.set(screen.x/2, screen.y/2);
		
		return false;
	}

	bool createWall(Core::GameContext& context, Core::Entity& wall)
	{
		return false;
	}

	bool createGoal(Core::GameContext& context, Core::Entity& goal)
	{
		return false;
	}
}
