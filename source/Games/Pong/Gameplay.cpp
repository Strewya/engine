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
#include "Core/Action/Impl/Physics.h"
#include "Core/Action/Impl/Render.h"
#include "Core/Entity/Entity.h"
#include "Games/Pong/Components.h"
#include "Games/Pong/EntityConstructors.h"
#include "Games/Pong/InputController.h"
#include "Games/Pong/Intents.h"
#include "Services/Graphics/IRenderer.h"
#include "Services/Input/Context.h"
#include "Services/Input/Engine.h"
#include "Services/Input/Event.h"
#include "Services/Input/KeyCodes.h"
#include "Util/Color.h"
#include "Util/Random.h"
	/*** end headers ***/

namespace Pong
{
	Gameplay::Gameplay(Core::ServiceLocator& services, Core::ResourceLocator& resources)
		: GameContext(Core::ContextType::GAMEPLAY, services, resources), m_debug(services.getGraphics())
	{
		m_b2ScalingFactor = 10;
		m_b2ScalingFactorInv = 1/m_b2ScalingFactor;
		uint32_t flags = 0;
		flags += Graphics::b2DebugDraw::e_shapeBit;
		flags += Graphics::b2DebugDraw::e_centerOfMassBit;
		flags += Graphics::b2DebugDraw::e_jointBit;
		flags += Graphics::b2DebugDraw::e_pairBit;
		//flags += Graphics::b2DebugDraw::e_aabbBit;
		m_debug.SetFlags(flags);
		m_debug.setLengthScale(m_b2ScalingFactor);
		m_physicsWorld.SetDebugDraw(&m_debug);
		m_render.reset(Core::MainRender::create(*this).release());
	}

	void Gameplay::registerActions()
	{
		setupAction(Core::InputController::create(*this), true, false);
		setupAction(Core::Physics2d::create(*this), true, true);
		setupAction(Core::Render::create(*this), false, true);
		
	}

	void Gameplay::registerCallbacks()
	{
		m_entityFactory.registerConstructor("paddle", createPaddle);
		m_entityFactory.registerConstructor("ball", createBall);
		m_entityFactory.registerConstructor("field", createField);

		//input
		m_keyBindings.createContext("main");
		Input::Context& main = m_keyBindings.getContext("main");
		m_keyBindings.pushContext("main", 1);

	}

	void Gameplay::createEntities()
	{
		auto& field = m_entityPool.getNewInstanceRef();
		m_entityFactory.createEntityType("field", field);
		m_allEntities.addEntity(field.getID());

		for(int i=0; i<2; ++i)
		{
			auto& paddle = m_entityPool.getNewInstanceRef();
			m_entityFactory.createEntityType("paddle", paddle);
			m_allEntities.addEntity(paddle.getID());
			
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
		
		auto& ball = m_entityPool.getNewInstanceRef();
		m_entityFactory.createEntityType("ball", ball);
		m_allEntities.addEntity(ball.getID());
		setupBall(ball);
	}

	void Gameplay::onActivate()
	{
		std::deque<InstanceID> entities;
		m_allEntities.findEntities(entities, m_entityPool, [](const Core::Entity& e)
		{
			return e.getType() == "ball";
		});
		assert(entities.size() == 1);
		setupBall(m_entityPool.getInstanceRef(entities[0]));
	}

	void Gameplay::onDeactivate()
	{
		
	}

	void Gameplay::destroyEntities()
	{
		for(auto eid : m_allEntities)
		{
			m_entityPool.destroy(eid);
		}
		m_allEntities.clear();
	}



	void Gameplay::setupLeftPaddle(Core::Entity& paddle)
	{
		paddle.setAlias("left");
		auto screenExtent = m_services.getGraphics().getScreenSize()/(2*m_b2ScalingFactor);
		
		auto* state = paddle.getState<Core::PhysicalBody>();
		state->m_body->SetTransform(b2Vec2(3-screenExtent.x, 0), 0);

		m_messenger.sendMessage(0, Core::InputController::UID, m_messenger.registerMessage("add_entity"), paddle.getID());
	}



	void Gameplay::setupRightPaddle(Core::Entity& paddle)
	{
		paddle.setAlias("right");
		auto screenExtent = m_services.getGraphics().getScreenSize()/(2*m_b2ScalingFactor);
		
		auto* state = paddle.getState<Core::PhysicalBody>();
		state->m_body->SetTransform(b2Vec2(screenExtent.x-3, 0), 0);
	}


	void Gameplay::setupBall(Core::Entity& ball)
	{
		auto* state = ball.getState<Core::PhysicalBody>();
		Util::Random random;
		auto y = random.randInt(0, 10);
		auto x = random.randInt(y, 25);
		state->m_body->ApplyLinearImpulse(b2Vec2((float)x,(float)y), state->m_body->GetWorldCenter());
	}

	void Gameplay::bindPaddleToField(Core::Entity& paddle, Core::Entity& field)
	{
		auto* paddleBody = paddle.getState<Core::PhysicalBody>();
		auto* fieldBody = field.getState<Core::PhysicalBody>();
		b2PrismaticJointDef jointDef;
		
		jointDef.collideConnected = true;
		jointDef.Initialize(paddleBody->m_body, fieldBody->m_body, paddleBody->m_body->GetWorldCenter(), b2Vec2(0, 1.0f));

		m_physicsWorld.CreateJoint(&jointDef);
	}
}

