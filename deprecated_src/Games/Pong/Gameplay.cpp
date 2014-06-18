//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include <stdafx.h>
	/*** personal header ***/
#include <Games/Pong/Gameplay.h>
	/*** C++ headers ***/
#include <map>
#include <memory>
#include <vector>
	/*** extra headers ***/
#include <Engine/GameEngine.h>
#include <Engine/ServiceLocator.h>
#include <Core/Entity/Entity.h>
#include <Core/State/CoreStates.h>
#include <Games/Pong/EntityConstructors.h>
#include <Modules/Input/InputAction.h>
#include <Modules/Input/Service/Context.h>
#include <Modules/Input/Service/InputEngine.h>
#include <Modules/Input/Service/Event.h>
#include <Modules/Input/Service/KeyCodes.h>
#include <Modules/Physics/PhysicsAction.h>
#include <Modules/Physics/PhysicsState.h>
#include <Modules/Physics/Service/PhysicsEngine.h>
#include <Modules/Graphics/RenderAction.h>
#include <Modules/Graphics/Service/IRenderer.h>
#include <Util/Color.h>
#include <Util/Random.h>
	/*** end headers ***/

namespace Pong
{
	Gameplay::Gameplay(Core::ServiceLocator& services, Core::ResourceLocator& resources)
		: GameContext(Core::ContextType::GAMEPLAY, services, resources)
	{
		m_render = Core::MainRender::create(*this);
		m_services.getPhysics().setScalingFactor(10);
		m_services.getGraphics().setScreenSize(1024, 768);
	}

	void Gameplay::registerActions()
	{
		setupAction(Core::InputController::create(*this), true, false);
		setupAction(Physics::PhysicsAction::create(*this), true, true);
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
        (void) main;

		main.addState(100, Input::Event(Input::DeviceCode::Mouse, Input::EventCode::Button, Input::Mouse::m_LeftButton, false));
	}

	void Gameplay::createEntities()
	{
		auto& field = m_entityPool.getNewInstanceRef();
		m_entityFactory.createEntityType("field", field);
		createEntity(Physics::PhysicsAction::UID, field.getID());

		for(int i=0; i<2; ++i)
		{
			auto& paddle = m_entityPool.getNewInstanceRef();
			m_entityFactory.createEntityType("paddle", paddle);
			createEntity(Physics::PhysicsAction::UID, paddle.getID());
			
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
		createEntity(Physics::PhysicsAction::UID, ball.getID());
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
			m_messenger.sendMessage(0, m_messenger.BROADCAST, m_messenger.encode("destroy_entity"), eid);
		}
		m_allEntities.clear();
	}



	void Gameplay::setupLeftPaddle(Core::Entity& paddle)
	{
		paddle.setAlias("left");
		auto screenExtent = m_services.getGraphics().getScreenSize()*0.5f;
		

		m_messenger.sendMessage(0, Core::InputController::UID, m_messenger.encode("add_entity"), paddle.getID());

		auto* positionState = paddle.getState<Core::Position2d>();
		auto* bodyState = paddle.getState<Physics::PhysicsBody>();
		positionState->m_position.set(-screenExtent.x*0.95f, 0);
		auto* body = m_services.getPhysics().getBody(bodyState->bodyID);
		
		body->SetTransform(Physics::convert(positionState->m_position, m_services.getPhysics().getInverseScalingFactor()), 0);
		
	}

	void Gameplay::setupRightPaddle(Core::Entity& paddle)
	{
		paddle.setAlias("right");
		auto screenExtent = m_services.getGraphics().getScreenSize()*0.5f;

		m_messenger.sendMessage(0, Core::InputController::UID, m_messenger.encode("add_entity"), paddle.getID());
		
		auto* positionState = paddle.getState<Core::Position2d>();
		auto* bodyState = paddle.getState<Physics::PhysicsBody>();
		positionState->m_position.set(screenExtent.x*0.95f, 0);
		auto* body = m_services.getPhysics().getBody(bodyState->bodyID);

		body->SetTransform(Physics::convert(positionState->m_position, m_services.getPhysics().getInverseScalingFactor()), 0);
	}

	void Gameplay::setupBall(Core::Entity& ball)
	{
		auto* state = ball.getState<Core::Impulse2d>();
		Util::Random random;
		auto y = random.randInt(90, 100);
		auto x = -random.randInt(y, 250);
		state->m_impulse.set((float)x, (float)y);
		m_messenger.sendMessage(0, Physics::PhysicsAction::UID, m_messenger.encode("apply_impulse"), ball.getID());
	}

	void Gameplay::bindPaddleToField(Core::Entity& paddle, Core::Entity& field)
	{
		auto& physicsEngine = m_services.getPhysics();
		auto* bodyAState = paddle.getState<Physics::PhysicsBody>();
		auto* bodyBState = field.getState<Physics::PhysicsBody>();

		auto* a = physicsEngine.getBody(bodyAState->bodyID);
		auto* b = physicsEngine.getBody(bodyBState->bodyID);

		b2PrismaticJointDef jointDef;
		jointDef.Initialize(a, b, b->GetWorldPoint(b2Vec2(0, 0)), b2Vec2(0, 1));
		jointDef.collideConnected = true;
		physicsEngine.createJoint(jointDef);
	}
}

