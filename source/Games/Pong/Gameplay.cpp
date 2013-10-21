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
#include "Engine/ServiceLocator.h"
#include "Core/Action/Impl/InputHandler.h"
#include "Core/Action/Impl/Physics.h"
#include "Core/Action/Impl/Render.h"
#include "Core/Entity/Entity.h"
#include "Games/Pong/Components.h"
#include "Games/Pong/EntityConstructors.h"
#include "Services/Graphics/IRenderer.h"
#include "Services/Input/Context.h"
#include "Services/Input/Engine.h"
#include "Services/Input/Event.h"
#include "Util/Color.h"
#include "Util/Random.h"
	/*** end headers ***/

namespace Pong
{
	Gameplay::Gameplay(Core::ServiceLocator& services, Core::ResourceLocator& resources)
		: GameContext(Core::ContextType::GAMEPLAY, services, resources), m_debug(services.getGraphics())
	{
		b2ScalingFactor = 10;
		physicsWorld.SetDebugDraw(&m_debug);
		uint32_t flags = 0;
		flags += Graphics::b2DebugDraw::e_shapeBit;
		flags += Graphics::b2DebugDraw::e_centerOfMassBit;
		flags += Graphics::b2DebugDraw::e_jointBit;
		flags += Graphics::b2DebugDraw::e_pairBit;
		//flags += Graphics::b2DebugDraw::e_aabbBit;
		m_debug.SetFlags(flags);
		m_debug.setLengthScale(b2ScalingFactor);
	}

	void Gameplay::registerActions()
	{
		actionRegistry.addAction(Core::Render::create(*this));
		actionRegistry.addAction(Core::Physics::create(*this));
		actionRegistry.addAction(Core::InputHandler::create(*this));
	}

	void Gameplay::setupActionQueue()
	{
		actionQueue.addAction(actionRegistry.getAction(Core::InputHandler::Type));
		actionQueue.addAction(actionRegistry.getAction(Core::Physics::Type));
		
		renderAction = &actionRegistry.getAction(Core::Render::Type);
	}

	void Gameplay::registerCallbacks()
	{
		entityFactory.registerConstructor("paddle", createPaddle);
		entityFactory.registerConstructor("ball", createBall);
		entityFactory.registerConstructor("field", createField);

		//input
		keyBindings.createContext("main");
		Input::Context& main = keyBindings.getContext("main");

	}

	void Gameplay::createEntities()
	{
		auto& field = entityPool.getNewInstanceRef();
		entityFactory.createEntityType("field", field);
		activeEntities.addEntity(field.getID());

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

			bindPaddleToField(paddle, field);
		}
		
		auto& ball = entityPool.getNewInstanceRef();
		entityFactory.createEntityType("ball", ball);
		activeEntities.addEntity(ball.getID());
		setupBall(ball);
	}

	void Gameplay::onActivate()
	{
		std::deque<InstanceID> entities;
		activeEntities.findEntities(entities, entityPool, [](const Core::Entity& e)
		{
			return e.getType() == "ball";
		});
		assert(entities.size() == 1);
		setupBall(entityPool.getInstanceRef(entities[0]));
	}

	void Gameplay::onDeactivate()
	{
		
	}

	void Gameplay::destroyEntities()
	{
		for(auto it = activeEntities.begin(); it != activeEntities.end(); ++it)
		{
			entityPool.destroy(*it);
		}
	}



	void Gameplay::setupLeftPaddle(Core::Entity& paddle)
	{
		paddle.setAlias("left");
		auto screenExtent = services.getGraphics().getScreenSize()/(2*b2ScalingFactor);
		
		auto* body = paddle.getState<Core::PhysicalBody>();
		body->data->SetTransform(b2Vec2(3-screenExtent.x, 0), 0);
	}



	void Gameplay::setupRightPaddle(Core::Entity& paddle)
	{
		paddle.setAlias("right");
		auto screenExtent = services.getGraphics().getScreenSize()/(2*b2ScalingFactor);
		
		auto* body = paddle.getState<Core::PhysicalBody>();
		body->data->SetTransform(b2Vec2(screenExtent.x-3, 0), 0);
	}


	void Gameplay::setupBall(Core::Entity& ball)
	{
		auto* body = ball.getState<Core::PhysicalBody>();
		Util::Random random;
		auto y = random.randInt(0, 20);
		auto x = random.randInt(y, 50);
		body->data->ApplyLinearImpulse(b2Vec2((float)x,(float)y), body->data->GetWorldCenter());
	}

	void Gameplay::bindPaddleToField(Core::Entity& paddle, Core::Entity& field)
	{
		auto* paddleBody = paddle.getState<Core::PhysicalBody>();
		auto* fieldBody = field.getState<Core::PhysicalBody>();
		b2PrismaticJointDef jointDef;
		
		jointDef.collideConnected = true;
		jointDef.Initialize(paddleBody->data, fieldBody->data, paddleBody->data->GetWorldCenter(), b2Vec2(0, 1.0f));

		physicsWorld.CreateJoint(&jointDef);
	}
}

