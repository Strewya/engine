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
		/**
			This function is called every time the context goes from being inactive to being the active context.
			It should create all of the entities that it needs to work properly.
		*/
		services.getGraphics().setBackgroundFillColor(127,127,127);

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
		/**
			This function is called every time the context goes from being the active one to being inactive.
			It should destroy all objects that are created in the ACTIVATE function.
			NOTE: Think of a way to keep entities beyond the inactivity boundary. 
				  The onActivate function should take this into consideration when creating that particular entity to check whether it already exists.
		*/
		for(auto it = activeEntities.begin(); it != activeEntities.end(); ++it)
		{
			entityPool.destroy(*it);
		}
	}


	void Gameplay::setupLeftPaddle(Core::Entity& paddle)
	{
		auto screen = services.getGraphics().getScreenSize();

		auto* pos = paddle.getState<Core::Position2d>();
		pos->value.Assign(10, screen.y/2);
	}


	void Gameplay::setupRightPaddle(Core::Entity& paddle)
	{
		auto screen = services.getGraphics().getScreenSize();

		auto* pos = paddle.getState<Core::Position2d>();
		pos->value.Assign(screen.x-10, screen.y/2);
	}


	void Gameplay::setupBall(Core::Entity& ball)
	{
	}
}

