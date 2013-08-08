//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Games/Pong/Gameplay.h"
	/*** C++ headers ***/
#include <map>
#include <memory>
#include <vector>
	/*** extra headers ***/
#include "Engine/Engine.h"
#include "Engine/GameContext.h"
#include "Core/Action/Impl/Render.h"
#include "Core/Entity/Entity.h"
#include "Games/Pong/Components.h"
#include "Subsystems/Graphics/IRenderer.h"
#include "Util/Color.h"
	/*** end headers ***/

namespace Pong
{
	Gameplay::Gameplay(Core::ServiceLocator& services, Core::ResourceLocator& resources)
		: GameContext(Core::ContextType::GAMEPLAY, services, resources)
	{}

	void Gameplay::registerActions()
	{
		actionIndex.addActionToIndex(Core::ARender::create());
	}


	void Gameplay::setupActionQueue()
	{
		actionQueue.addAction(actionIndex.getActionFromIndex(Core::ARender::Type));
	}


	void Gameplay::activate()
	{
		services.getGraphics().setBackgroundColor((uint8_t)127,127,127);
		auto c = services.getGraphics().getBackgroundColor();
		assert(c.getRed() == 127 && c.getGreen() == 127 && c.getBlue() == 127);
		
		for(int i=0; i<2; ++i)
		{
			auto& paddle = entityPool.getNewInstanceRef();
			entityFactory.createEntityType("paddle", paddle);
			activeEntities.addEntity(paddle.getID());

			if(i == 0)
			{
				setupLeftPaddle(paddle);
			}
			else
			{
				setupRightPaddle(paddle);
			}
		}
		
		auto& ball = entityPool.getNewInstanceRef();
		entityFactory.createEntityType("ball", ball);
		activeEntities.addEntity(ball.getID());
		
		setupBall(ball);
	}


	void Gameplay::deactivate()
	{

		for(auto it = activeEntities.begin(); it != activeEntities.end(); ++it)
		{
			entityPool.destroy(*it);
		}
	}


	void Gameplay::setupLeftPaddle(Core::Entity& paddle)
	{
		//auto screen = Util::Vec2();
		auto screen = services.getGraphics().getScreenSize();

		auto* position = paddle.getState<Core::Position2d>();
		position->value.y = screen.y/2;
	}


	void Gameplay::setupRightPaddle(Core::Entity& paddle)
	{
		//auto screen = Util::Vec2();
		auto screen = services.getGraphics().getScreenSize();

		auto* position = paddle.getState<Core::Position2d>();
		position->value.y = screen.y/2;
	}


	void Gameplay::setupBall(Core::Entity& ball)
	{
	}
}

