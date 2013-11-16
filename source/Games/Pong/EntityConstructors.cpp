	/*** precompiled header ***/
#include <stdafx.h>
	/*** personal header ***/
#include <Games/Pong/EntityConstructors.h>
	/*** extra headers ***/
#include <Core/Action/Impl/Physics.h>
#include <Engine/GameContext.h>
#include <Engine/ServiceLocator.h>
#include <Games/Pong/Components.h>
#include <Services/Graphics/IRenderer.h>
	/*** end headers ***/

namespace Pong
{
	bool createPaddle(Core::GameContext& context, Core::Entity& paddle)
	{
		auto screenExtent = context.m_services.getGraphics().getScreenSize()/(2*10);
		auto& position = paddle.insert(Core::Position2d::create());
		auto& body = paddle.insert(Core::PhysicalBody::create());
		auto& velocity = paddle.insert(Core::Velocity2d::create());

		velocity.m_maxVelocity.set(0, 20);


		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(0,0);
		bodyDef.gravityScale = 0;
		bodyDef.fixedRotation = true;
		bodyDef.userData = &paddle;
		//body.m_body = physics->m_physicsWorld.CreateBody(&bodyDef);
		

		b2PolygonShape shape;
		shape.SetAsBox(1, screenExtent.y*0.2f);

		b2FixtureDef fixtDef;
		fixtDef.shape = &shape;
		fixtDef.density = 1;

		b2Fixture* fixture = body.m_body->CreateFixture(&fixtDef);

		return true;
	}

	bool createBall(Core::GameContext& context, Core::Entity& ball)
	{
		auto screenExtent = context.m_services.getGraphics().getScreenSize()/(2*10);
		auto& position = ball.insert(Core::Position2d::create());
		auto& body = ball.insert(Core::PhysicalBody::create());
		auto& velocity = ball.insert(Core::Velocity2d::create());

		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(0,0);
		bodyDef.gravityScale = 0;
		bodyDef.userData = &ball;
		//body.m_body = physics->m_physicsWorld.CreateBody(&bodyDef);

		b2CircleShape shape;
		shape.m_p.Set(0,0);
		shape.m_radius = 1.2f;

		b2FixtureDef fixtDef;
		fixtDef.shape = &shape;
		fixtDef.density = 0.5f;
		fixtDef.restitution = 1;
		fixtDef.friction = 0;
		
		b2Fixture* fixture = body.m_body->CreateFixture(&fixtDef);

		return true;
	}

	bool createField(Core::GameContext& context, Core::Entity& field)
	{
		auto screenExtent = context.m_services.getGraphics().getScreenSize()/(2*10);
		auto& body = field.insert(Core::PhysicalBody::create());

		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(0,0);
		bodyDef.userData = &field;
		//body.m_body = physics->m_physicsWorld.CreateBody(&bodyDef);

		//the field has 4 fixtures, 2 for the top/bottom walls, and 2 for the goals
		b2PolygonShape shape;
		b2FixtureDef fixtDef;
		fixtDef.density = 1;
		fixtDef.shape = &shape;

		//left goal
		shape.SetAsBox(1, screenExtent.y, b2Vec2(-screenExtent.x, 0), 0);
		b2Fixture* leftGoal = body.m_body->CreateFixture(&fixtDef);

		//right goal
		shape.SetAsBox(1, screenExtent.y, b2Vec2(screenExtent.x, 0), 0);
		b2Fixture* rightGoal = body.m_body->CreateFixture(&fixtDef);

		//top wall
		shape.SetAsBox(screenExtent.x, 1, b2Vec2(0, screenExtent.y-1), 0);
		b2Fixture* topWall = body.m_body->CreateFixture(&fixtDef);

		//bottom wall
		shape.SetAsBox(screenExtent.x, 1, b2Vec2(0, 1-screenExtent.y), 0);
		b2Fixture* bottomWall = body.m_body->CreateFixture(&fixtDef);

		

		

		return true;
	}
}
