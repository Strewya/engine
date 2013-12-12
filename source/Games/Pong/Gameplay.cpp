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
#include <Engine/Engine.h>
#include <Engine/ServiceLocator.h>
#include <Core/Entity/Entity.h>
#include <Core/State/CoreStates.h>
#include <Games/Pong/EntityConstructors.h>
#include <Modules/Input/InputAction.h>
#include <Modules/Input/Service/Context.h>
#include <Modules/Input/Service/InputEngine.h>
#include <Modules/Input/Service/Event.h>
#include <Modules/Input/Service/KeyCodes.h>
#include <Modules/Physics/Box2D/Box2DAction.h>
#include <Modules/Physics/Box2D/Box2DEngine.h>
#include <Modules/Physics/Box2D/Box2DState.h>
#include <Modules/Rendering/RenderAction.h>
#include <Modules/Rendering/Service/IRenderer.h>
#include <Util/Color.h>
#include <Util/Random.h>
	/*** end headers ***/

namespace Pong
{
	Gameplay::Gameplay(Core::ServiceLocator& services, Core::ResourceLocator& resources)
		: GameContext(Core::ContextType::GAMEPLAY, services, resources)
	{
		m_render = Core::MainRender::create(*this);
	}

	void Gameplay::registerActions()
	{
		setupAction(Core::InputController::create(*this), true, false);
		setupAction(Physics::Physics2d::create(*this), true, true);
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
	}

	void Gameplay::createEntities()
	{
		auto& field = m_entityPool.getNewInstanceRef();
		m_entityFactory.createEntityType("field", field);
		m_allEntities.addEntity(field.getID());
		createEntity(Physics::Physics2d::UID, field.getID());

		for(int i=0; i<2; ++i)
		{
			auto& paddle = m_entityPool.getNewInstanceRef();
			m_entityFactory.createEntityType("paddle", paddle);
			m_allEntities.addEntity(paddle.getID());
			createEntity(Physics::Physics2d::UID, paddle.getID());
			
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
		createEntity(Physics::Physics2d::UID, ball.getID());
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
		auto screenExtent = m_services.getGraphics().getScreenSize()/(2);
		
		m_messenger.sendMessage(0, Core::InputController::UID, m_messenger.encode("add_entity"), paddle.getID());

		auto* state = paddle.getState<Core::Position2d>();
		state->m_position.set(3-screenExtent.x, 0);
		m_messenger.sendMessage(0, Physics::Physics2d::UID, m_messenger.encode("set_position"), paddle.getID());
		
	}

	void Gameplay::setupRightPaddle(Core::Entity& paddle)
	{
		paddle.setAlias("right");
		auto screenExtent = m_services.getGraphics().getScreenSize()/(2);

		m_messenger.sendMessage(0, Core::InputController::UID, m_messenger.encode("add_entity"), paddle.getID());
		
		auto* state = paddle.getState<Core::Position2d>();
		state->m_position.set(screenExtent.x-3, 0);
		m_messenger.sendMessage(0, Physics::Physics2d::UID, m_messenger.encode("set_position"), paddle.getID());
	}

	void Gameplay::setupBall(Core::Entity& ball)
	{
		auto* state = ball.getState<Core::Impulse2d>();
		Util::Random random;
		auto y = random.randInt(0, 10);
		auto x = random.randInt(y, 25);
		state->m_impulse.set((float)x, (float)y);
	}

	void Gameplay::bindPaddleToField(Core::Entity& paddle, Core::Entity& field)
	{
		auto& joints = paddle.insert(Physics::Box2dJoint::create());
		
		joints.m_joints.resize(1);
        joints.m_joints[0].m_prismatic.collideConnected = true;
        joints.m_joints[0].m_bodyA = paddle.getID();
		joints.m_joints[0].m_bodyB = field.getID();

		joints.m_joints[0].m_initDefinition = [](Physics::JointData& data, b2Body* a, b2Body* b)
		{
            data.m_prismatic.Initialize(a, b, b->GetWorldPoint(b2Vec2(0,0)), b2Vec2(0,1));
		};

		m_messenger.sendMessage(0, Physics::Physics2d::UID, m_messenger.encode("create_joints"), paddle.getID());
	}
}

