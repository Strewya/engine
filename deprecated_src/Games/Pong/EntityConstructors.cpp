	/*** precompiled header ***/
#include <stdafx.h>
	/*** personal header ***/
#include <Games/Pong/EntityConstructors.h>
	/*** extra headers ***/
#include <Core/State/CoreStates.h>
#include <Engine/GameContext.h>
#include <Engine/ServiceLocator.h>
#include <Modules/Physics/Service/PhysicsEngine.h>
#include <Modules/Physics/PhysicsAction.h>
#include <Modules/Physics/PhysicsState.h>
#include <Modules/Graphics/Service/IRenderer.h>
	/*** end headers ***/

namespace Pong
{
	bool createPaddle(Core::GameContext& context, Core::Entity& paddle)
	{
		auto& physicsEngine = context.m_services.getPhysics();
		auto screenExtent = Physics::convert(context.m_services.getGraphics().getScreenSize()*0.5f, physicsEngine.getInverseScalingFactor());
        /*auto& position = */paddle.insert(Core::Position2d::create());
		auto& body = paddle.insert(Physics::PhysicsBody::create());
		auto& velocity = paddle.insert(Core::Velocity2d::create());

		velocity.m_maxVelocity.set(0, 20);
		body.fixtureCount = 0;

		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(0, 0);
		bodyDef.gravityScale = 0;
		bodyDef.fixedRotation = true;
		bodyDef.userData = (void*)paddle.getID();
		body.bodyID = physicsEngine.createBody(bodyDef);
		
		b2FixtureDef fixtureDef;
		b2PolygonShape polygon;
		fixtureDef.shape = &polygon;
		polygon.SetAsBox(1, screenExtent.y*0.2f);
		fixtureDef.density = 1;
		body.fixtureID[body.fixtureCount++] = physicsEngine.createFixture(fixtureDef, body.bodyID);
		
		return true;
	}

	bool createBall(Core::GameContext& context, Core::Entity& ball)
	{
		auto& physicsEngine = context.m_services.getPhysics();
        /*auto& position = */ball.insert(Core::Position2d::create());
		auto& body = ball.insert(Physics::PhysicsBody::create());
        /*auto& velocity = */ball.insert(Core::Velocity2d::create());
        /*auto& impulse = */ball.insert(Core::Impulse2d::create());

		body.fixtureCount = 0;

		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(0, 0);
		bodyDef.gravityScale = 0;
		bodyDef.userData = (void*)ball.getID();
		body.bodyID = physicsEngine.createBody(bodyDef);
		
		b2FixtureDef fixtureDef;
		b2CircleShape circle;
		fixtureDef.shape = &circle;
		circle.m_p.Set(0, 0);
		circle.m_radius = 1;
		fixtureDef.density = 0.5f;
		fixtureDef.restitution = 1;
		fixtureDef.friction = 0;
		body.fixtureID[body.fixtureCount++] = physicsEngine.createFixture(fixtureDef, body.bodyID);
		
		return true;
	}

	bool createField(Core::GameContext& context, Core::Entity& field)
	{
		auto& physicsEngine = context.m_services.getPhysics();
		auto screenExtent = Physics::convert(context.m_services.getGraphics().getScreenSize()*0.5f, physicsEngine.getInverseScalingFactor());
		auto& body = field.insert(Physics::PhysicsBody::create());
		auto& position = field.insert(Core::Position2d::create());

		body.fixtureCount = 0;
		
		position.m_position.set(0,0);
		
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(0, 0);
		bodyDef.userData = (void*)field.getID();
		body.bodyID = physicsEngine.createBody(bodyDef);

		b2FixtureDef fixtureDef;
		b2PolygonShape polygon;
		fixtureDef.shape = &polygon;
		fixtureDef.density = 1;
		
		polygon.SetAsBox(1, screenExtent.y, b2Vec2(-screenExtent.x, 0), 0);
		body.fixtureID[body.fixtureCount++] = physicsEngine.createFixture(fixtureDef, body.bodyID);

		polygon.SetAsBox(1, screenExtent.y, b2Vec2(screenExtent.x, 0), 0);
		body.fixtureID[body.fixtureCount++] = physicsEngine.createFixture(fixtureDef, body.bodyID);

		polygon.SetAsBox(screenExtent.x, 1, b2Vec2(0, screenExtent.y - 1), 0);
		body.fixtureID[body.fixtureCount++] = physicsEngine.createFixture(fixtureDef, body.bodyID);

		polygon.SetAsBox(screenExtent.x, 1, b2Vec2(0, 1 - screenExtent.y), 0);
		body.fixtureID[body.fixtureCount++] = physicsEngine.createFixture(fixtureDef, body.bodyID);

		return true;
	}
}
