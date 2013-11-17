	/*** precompiled header ***/
#include <stdafx.h>
	/*** personal header ***/
#include <Games/Pong/EntityConstructors.h>
	/*** extra headers ***/
#include <Core/Action/Impl/Physics.h>
#include <Core/State/GeneralComponents.h>
#include <Core/State/Impl/Box2dPhysics.h>
#include <Engine/GameContext.h>
#include <Engine/ServiceLocator.h>
#include <Services/Graphics/IRenderer.h>
	/*** end headers ***/

namespace Pong
{
	bool createPaddle(Core::GameContext& context, Core::Entity& paddle)
	{
		auto screenExtent = context.m_services.getGraphics().getScreenSize()/(2);
        /*auto& position = */paddle.insert(Core::Position2d::create());
		auto& body = paddle.insert(Core::Box2dBodyDef::create());
		auto& fixtures = paddle.insert(Core::Box2dFixtures::create());
		auto& velocity = paddle.insert(Core::Velocity2d::create());

		velocity.m_maxVelocity.set(0, 20);

		body.m_def.type = b2_dynamicBody;
		body.m_def.position.Set(0,0);
		body.m_def.gravityScale = 0;
		body.m_def.fixedRotation = true;
		body.m_def.userData = (void*)paddle.getID();
		context.m_messenger.sendMessage(0, Core::Physics2d::UID, context.m_messenger.encode("create_body"), paddle.getID());


		fixtures.m_fixtures.resize(1);
		fixtures.m_fixtures[0].m_def.shape = &fixtures.m_fixtures[0].m_polygon;
		fixtures.m_fixtures[0].m_polygon.SetAsBox(1, screenExtent.y*0.2f);
		//fixtures.m_fixtures[0].m_def.shape = &fixtures.m_fixtures[0].m_polygon;
		fixtures.m_fixtures[0].m_def.density = 1;
		context.m_messenger.sendMessage(0, Core::Physics2d::UID, context.m_messenger.encode("create_fixtures"), paddle.getID());

		return true;
	}

	bool createBall(Core::GameContext& context, Core::Entity& ball)
	{
        /*auto& position = */ball.insert(Core::Position2d::create());
		auto& body = ball.insert(Core::Box2dBodyDef::create());
		auto& fixtures = ball.insert(Core::Box2dFixtures::create());
        /*auto& velocity = */ball.insert(Core::Velocity2d::create());
        /*auto& impulse = */ball.insert(Core::Impulse2d::create());


		body.m_def.type = b2_dynamicBody;
		body.m_def.position.Set(0,0);
		body.m_def.gravityScale = 0;
		body.m_def.userData = (void*)ball.getID();
		context.m_messenger.sendMessage(0, Core::Physics2d::UID, context.m_messenger.encode("create_body"), ball.getID());

		fixtures.m_fixtures.resize(1);
		fixtures.m_fixtures[0].m_def.shape = &fixtures.m_fixtures[0].m_circle;
		fixtures.m_fixtures[0].m_circle.m_p.Set(0,0);
		fixtures.m_fixtures[0].m_circle.m_radius = 1.2f;
		//fixtures.m_fixtures[0].m_def.shape = &fixtures.m_fixtures[0].m_circle;
		fixtures.m_fixtures[0].m_def.density = 0.5f;
		fixtures.m_fixtures[0].m_def.restitution = 1;
		fixtures.m_fixtures[0].m_def.friction = 0;
		context.m_messenger.sendMessage(0, Core::Physics2d::UID, context.m_messenger.encode("create_fixtures"), ball.getID());

		return true;
	}

	bool createField(Core::GameContext& context, Core::Entity& field)
	{
		auto screenExtent = context.m_services.getGraphics().getScreenSize()/(2);
		auto& body = field.insert(Core::Box2dBodyDef::create());
		auto& position = field.insert(Core::Position2d::create());
		auto& fixtures = field.insert(Core::Box2dFixtures::create());

		position.m_position.set(0,0);
		body.m_def.type = b2_staticBody;
		body.m_def.position.Set(0,0);
		body.m_def.userData = (void*)field.getID();
		context.m_messenger.sendMessage(0, Core::Physics2d::UID, context.m_messenger.encode("create_body"), field.getID());

		//the field has 4 fixtures, 2 for the top/bottom walls, and 2 for the goals
		fixtures.m_fixtures.resize(4);
		for(int i = 0; i<4; ++i)
		{
			fixtures.m_fixtures[i].m_def.shape = &fixtures.m_fixtures[i].m_polygon;
			fixtures.m_fixtures[i].m_def.density = 1;
			//fixtures.m_fixtures[i].m_def.shape = &fixtures.m_fixtures[i].m_polygon;
		}
		//left goal
		fixtures.m_fixtures[0].m_polygon.SetAsBox(1, screenExtent.y, b2Vec2(-screenExtent.x, 0), 0);
		
		//right goal
		fixtures.m_fixtures[1].m_polygon.SetAsBox(1, screenExtent.y, b2Vec2(screenExtent.x, 0), 0);
		
		//top wall
		fixtures.m_fixtures[2].m_polygon.SetAsBox(screenExtent.x, 1, b2Vec2(0, screenExtent.y-1), 0);
		
		//bottom wall
		fixtures.m_fixtures[3].m_polygon.SetAsBox(screenExtent.x, 1, b2Vec2(0, 1-screenExtent.y), 0);
		
		context.m_messenger.sendMessage(0, Core::Physics2d::UID, context.m_messenger.encode("create_fixtures"), field.getID());

		

		return true;
	}
}
