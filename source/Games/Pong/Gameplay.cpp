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
#include "Engine/GameContext.h"
#include "Engine/ServiceLocator.h"
#include "Core/Action/Impl/Physics.h"
#include "Core/Action/Impl/Render.h"
#include "Core/Entity/Entity.h"
#include "Games/Pong/Components.h"
#include "Games/Pong/InputHandler.h"
#include "Services/Graphics/IRenderer.h"
#include "Util/Color.h"
	/*** end headers ***/

namespace Pong
{
	Gameplay::Gameplay(Core::ServiceLocator& services, Core::ResourceLocator& resources)
		: GameContext(Core::ContextType::GAMEPLAY, services, resources), m_debug(services.getGraphics())
	{
		b2ScalingFactor = 10;
	}

	void Gameplay::registerActions()
	{
		actionIndex.addActionToIndex(Core::Render::create(*this));
		actionIndex.addActionToIndex(Core::Physics::create(*this));
		actionIndex.addActionToIndex(Pong::InputHandler::create(*this));
	}


	void Gameplay::setupActionQueue()
	{
		actionQueue.addAction(actionIndex.getActionFromIndex(Pong::InputHandler::Type));
		actionQueue.addAction(actionIndex.getActionFromIndex(Core::Physics::Type));
		
		renderAction = &actionIndex.getActionFromIndex(Core::Render::Type);
	}

	void Gameplay::activate()
	{
		physicsWorld.SetDebugDraw(&m_debug);
		uint32_t flags = 0;
		flags += Graphics::b2DebugDraw::e_shapeBit;
		flags += Graphics::b2DebugDraw::e_centerOfMassBit;
		flags += Graphics::b2DebugDraw::e_jointBit;
		flags += Graphics::b2DebugDraw::e_pairBit;
		//flags += Graphics::b2DebugDraw::e_aabbBit;
		m_debug.SetFlags(flags);
		m_debug.setLengthScale(b2ScalingFactor);

		services.getGraphics().setBackgroundColor(0.5f,0.5f,0.5f);
		auto c = services.getGraphics().getBackgroundColor();
		assert(c.getRed() == 0.5f && c.getGreen() == 0.5f && c.getBlue() == 0.5f);
		
		for(int i=0; i<2; ++i)
		{
			auto& paddle = entityPool.getNewInstanceRef();
			auto& wall = entityPool.getNewInstanceRef();
			auto& goal = entityPool.getNewInstanceRef();

			entityFactory.createEntityType("paddle", paddle);
			entityFactory.createEntityType("wall", wall);
			entityFactory.createEntityType("goal", goal);

			activeEntities.addEntity(paddle.getID());
			activeEntities.addEntity(wall.getID());
			activeEntities.addEntity(goal.getID());

			if(i == 0)
			{
				setupLeftPaddle(paddle);
				setupTopWall(wall);
				setupLeftGoal(goal);
			}
			else
			{
				setupRightPaddle(paddle);
				setupBottomWall(wall);
				setupRightGoal(goal);
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
		auto screenExtent = services.getGraphics().getScreenSize()/(2*b2ScalingFactor);
		
		auto* body = paddle.getState<Core::PhysicalBody>();
		body->data->SetTransform(b2Vec2(3-screenExtent.x, 0), 0);
	}


	void Gameplay::setupRightPaddle(Core::Entity& paddle)
	{
		auto screenExtent = services.getGraphics().getScreenSize()/(2*b2ScalingFactor);
		
		auto* body = paddle.getState<Core::PhysicalBody>();
		body->data->SetTransform(b2Vec2(screenExtent.x-3, 0), 0);
	}


	void Gameplay::setupBall(Core::Entity& ball)
	{
		auto* body = ball.getState<Core::PhysicalBody>();
		body->data->ApplyLinearImpulse(b2Vec2(150,2), body->data->GetWorldCenter());
	}

	void Gameplay::setupTopWall(Core::Entity& wall)
	{
		auto screenExtent = services.getGraphics().getScreenSize()/(2*b2ScalingFactor);
		
		auto* body = wall.getState<Core::PhysicalBody>();
		body->data->SetTransform(b2Vec2(0, screenExtent.y-1), 0);
	}

	void Gameplay::setupBottomWall(Core::Entity& wall)
	{
		auto screenExtent = services.getGraphics().getScreenSize()/(2*b2ScalingFactor);
		
		auto* body = wall.getState<Core::PhysicalBody>();
		body->data->SetTransform(b2Vec2(0, 1-screenExtent.y), 0);
	}

	void Gameplay::setupLeftGoal(Core::Entity& goal)
	{
		auto screenExtent = services.getGraphics().getScreenSize()/(2*b2ScalingFactor);

		auto* body = goal.getState<Core::PhysicalBody>();
		body->data->SetTransform(b2Vec2(-screenExtent.x, 0), 0);
	}

	void Gameplay::setupRightGoal(Core::Entity& goal)
	{
		auto screenExtent = services.getGraphics().getScreenSize()/(2*b2ScalingFactor);

		auto* body = goal.getState<Core::PhysicalBody>();
		body->data->SetTransform(b2Vec2(screenExtent.x, 0), 0);
	}
}

