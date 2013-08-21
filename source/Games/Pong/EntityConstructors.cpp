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
		
		return false;
	}

	bool createBall(Core::GameContext& context, Core::Entity& ball)
	{
		auto screen = context.services.getGraphics().getScreenSize();
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
		shape.m_radius = 1;

		b2FixtureDef fixtDef;
		fixtDef.shape = &shape;
		fixtDef.density = 1;
		fixtDef.restitution = 1;


		b2Fixture* fixture = body.data->CreateFixture(&fixtDef);

		body.data->ApplyForceToCenter(b2Vec2(0,2000));
		
		return true;
	}

	bool createWall(Core::GameContext& context, Core::Entity& wall)
	{
		auto& pos = wall.insert(Core::Position2d::create());
		auto& body = wall.insert(Core::PhysicalBody::create());


		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(0,0);
		bodyDef.userData = &wall;
		body.data = context.physicsWorld.CreateBody(&bodyDef);

		b2PolygonShape shape;
		shape.SetAsBox(50, 10);

		b2FixtureDef fixtDef;
		fixtDef.shape = &shape;
		fixtDef.density = 1;

		b2Fixture* fixture = body.data->CreateFixture(&fixtDef);

		return true;
	}

	bool createGoal(Core::GameContext& context, Core::Entity& goal)
	{
		return false;
	}
}
