	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Games/Pong/EntityConstructors.h"
	/*** extra headers ***/
#include "Engine/GameContext.h"
#include "Engine/ServiceLocator.h"
#include "Games/Pong/Components.h"
#include "Services/Graphics/IRenderer.h"
	/*** end headers ***/

namespace Pong
{
	bool createPaddle(Core::GameContext& context, Core::Entity& paddle)
	{
		auto screenExtent = context.services.getGraphics().getScreenSize()/(2*context.b2ScalingFactor);
		auto& pos = paddle.insert(Core::Position2d::create());
		auto& body = paddle.insert(Core::PhysicalBody::create());


		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(0,0);
		bodyDef.gravityScale = 0;
		bodyDef.userData = &paddle;
		body.data = context.physicsWorld.CreateBody(&bodyDef);

		b2PolygonShape shape;
		shape.SetAsBox(1, screenExtent.y*0.2f);

		b2FixtureDef fixtDef;
		fixtDef.shape = &shape;
		fixtDef.density = 1000;

		b2Fixture* fixture = body.data->CreateFixture(&fixtDef);



		return true;
	}

	bool createBall(Core::GameContext& context, Core::Entity& ball)
	{
		auto screenExtent = context.services.getGraphics().getScreenSize()/(2*context.b2ScalingFactor);
		auto& pos = ball.insert(Core::Position2d::create());
		auto& body = ball.insert(Core::PhysicalBody::create());


		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(0,0);
		bodyDef.gravityScale = 0;
		bodyDef.userData = &ball;
		body.data = context.physicsWorld.CreateBody(&bodyDef);

		b2CircleShape shape;
		shape.m_p.Set(0,0);
		shape.m_radius = 1.5;

		b2FixtureDef fixtDef;
		fixtDef.shape = &shape;
		fixtDef.density = 0;
		fixtDef.restitution = 1;
		fixtDef.friction = 0;


		b2Fixture* fixture = body.data->CreateFixture(&fixtDef);

		return true;
	}

	bool createWall(Core::GameContext& context, Core::Entity& wall)
	{
		auto screenExtent = context.services.getGraphics().getScreenSize()/(2*context.b2ScalingFactor);
		auto& pos = wall.insert(Core::Position2d::create());
		auto& body = wall.insert(Core::PhysicalBody::create());


		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(0,0);
		bodyDef.userData = &wall;
		body.data = context.physicsWorld.CreateBody(&bodyDef);

		b2PolygonShape shape;
		shape.SetAsBox(screenExtent.x, 1);

		b2FixtureDef fixtDef;
		fixtDef.shape = &shape;
		fixtDef.density = 1;

		b2Fixture* fixture = body.data->CreateFixture(&fixtDef);

		return true;
	}

	bool createGoal(Core::GameContext& context, Core::Entity& goal)
	{
		auto screenExtent = context.services.getGraphics().getScreenSize()/(2*context.b2ScalingFactor);
		auto& pos = goal.insert(Core::Position2d::create());
		auto& body = goal.insert(Core::PhysicalBody::create());


		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(0,0);
		bodyDef.userData = &goal;
		body.data = context.physicsWorld.CreateBody(&bodyDef);

		b2PolygonShape shape;
		shape.SetAsBox(1, screenExtent.y);

		b2FixtureDef fixtDef;
		fixtDef.shape = &shape;
		fixtDef.density = 1;

		b2Fixture* fixture = body.data->CreateFixture(&fixtDef);

		return true;
	}
}
