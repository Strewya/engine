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
#include "Services/Graphics/IRenderer.h"
#include "Util/Color.h"
	/*** end headers ***/

namespace Pong
{
	Gameplay::Gameplay(Core::ServiceLocator& services, Core::ResourceLocator& resources)
		: GameContext(Core::ContextType::GAMEPLAY, services, resources), debug(services.getGraphics())
	{
		physicsWorld.SetGravity(b2Vec2(0, -9.81f));
	}

	void Gameplay::registerActions()
	{
		actionIndex.addActionToIndex(Core::ARender::create());
	}


	void Gameplay::setupActionQueue()
	{
		actionQueue.addAction(actionIndex.getActionFromIndex(Core::ARender::Type));
	}

	static b2Body* dyno = nullptr;
	static b2Body* floor = nullptr;

	void Gameplay::activate()
	{
		physicsWorld.SetDebugDraw(&debug);
		uint32_t flags = 0;
		flags += Graphics::b2DebugDraw::e_shapeBit;
		flags += Graphics::b2DebugDraw::e_centerOfMassBit;
		flags += Graphics::b2DebugDraw::e_jointBit;
		flags += Graphics::b2DebugDraw::e_pairBit;
		//flags += Graphics::b2DebugDraw::e_aabbBit;
		debug.SetFlags(flags);
		debug.setLengthScale(10);

		services.getGraphics().setBackgroundColor(0.5f,0.5f,0.5f);
		auto c = services.getGraphics().getBackgroundColor();
		assert(c.getRed() == 0.5f && c.getGreen() == 0.5f && c.getBlue() == 0.5f);
		
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

		timer.RegisterTimer(myTimer);
		myTimer.setUpdatePeriod(1.0f/60.0f);

		if(dyno == nullptr && floor == nullptr)
		{
			b2BodyDef bodyDef;
			bodyDef.type = b2_dynamicBody;
			bodyDef.position = b2Vec2(0,20);
			bodyDef.angle = 0;
			//...
			dyno = physicsWorld.CreateBody(&bodyDef);

			b2PolygonShape shape;
			shape.SetAsBox(1,1);

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &shape;
			fixtureDef.density = 1;
			fixtureDef.restitution = 0.8f;
			dyno->CreateFixture(&fixtureDef);


			//floor

			bodyDef.type = b2_staticBody;
			bodyDef.position = b2Vec2(0,0);
			floor = physicsWorld.CreateBody(&bodyDef);

			shape.SetAsBox(3,0.5f);
			fixtureDef.shape = &shape;
			floor->CreateFixture(&fixtureDef);
		}
	}


	void Gameplay::deactivate()
	{
		for(auto it = activeEntities.begin(); it != activeEntities.end(); ++it)
		{
			entityPool.destroy(*it);
		}

		if(dyno != nullptr && floor != nullptr)
		{
			physicsWorld.DestroyBody(dyno);
			physicsWorld.DestroyBody(floor);
		}
	}

	void Gameplay::onUpdate(float dt)
	{
		while(myTimer.HasUpdatePeriodElapsed())
		{
			physicsWorld.Step(myTimer.getUpdatePeriod(), 8, 3);
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

