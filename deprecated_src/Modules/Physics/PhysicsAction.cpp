//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include <stdafx.h>
	/*** personal header ***/
#include <Modules/Physics/PhysicsAction.h>
	/*** C++ headers ***/
	/*** extra headers ***/
#include <Core/State/CoreStates.h>
#include <Engine/GameContext.h>
#include <Engine/ServiceLocator.h>
#include <Modules/Physics/Service/PhysicsEngine.h>
#include <Modules/Physics/PhysicsState.h>
	/*** end headers ***/
	
using Core::GameContext;

namespace Physics
{
	void PhysicsAction::init(GameContext& context)
	{
		auto& physicsEngine = context.m_services.getPhysics();

		physicsEngine.m_world.SetAllowSleeping(true);
		
		uint32_t flags = 0;
		flags += Graphics::b2DebugDraw::e_shapeBit;
		flags += Graphics::b2DebugDraw::e_centerOfMassBit;
		flags += Graphics::b2DebugDraw::e_jointBit;
		flags += Graphics::b2DebugDraw::e_pairBit;
		//flags += Graphics::b2DebugDraw::e_aabbBit;
		
		m_debug.SetFlags(flags);
		m_debug.setLengthScale(physicsEngine.getScalingFactor());
		m_debug.setRenderer(context.m_services.getGraphics());
		
		physicsEngine.m_world.SetDebugDraw(&m_debug);

		m_msgCreateBody = context.m_messenger.encode("create_body");
		m_msgCreateFixtures = context.m_messenger.encode("create_fixtures");
		m_msgCreateJoint = context.m_messenger.encode("create_joints");
		m_msgSetPosition = context.m_messenger.encode("set_position");
		m_msgApplyImpulse = context.m_messenger.encode("apply_impulse");
	}

	void PhysicsAction::processMessage(GameContext& context, uint32_t msg, InstanceID entityID)
	{
		if (!context.m_entityPool.isAlive(entityID)) return;
		auto& e = context.m_entityPool.getInstanceRef(entityID);
		
		if (msg == m_msgApplyImpulse)
		{
			auto* bodyState = e.getState<PhysicsBody>();
			auto* body = context.m_services.getPhysics().getBody(bodyState->bodyID);
			
			auto* impulse = e.getState<Core::Impulse2d>();
			if (impulse != nullptr)
			{
				body->ApplyLinearImpulse(convert(impulse->m_impulse, context.m_services.getPhysics().getInverseScalingFactor()), body->GetWorldCenter());
			}
		}
		else if (msg == 100)
		{
			auto* bodyState = e.getState<PhysicsBody>();
			auto* body = context.m_services.getPhysics().getBody(bodyState->bodyID);
			body->ApplyForce(b2Vec2(0, 10), body->GetWorldCenter());
		}
	}

	void PhysicsAction::update(GameContext& context)
	{
		//step the physics
		const int32_t velocityIterations = 8;
		const int32_t positionIterations = 3;
		context.m_services.getPhysics().m_world.Step(context.m_timer.getDeltaTime(), velocityIterations, positionIterations);
	}

	void PhysicsAction::render(GameContext& context, uint64_t interpolationTime)
	{
		context.m_services.getPhysics().m_world.DrawDebugData();
	}

	bool PhysicsAction::validateEntity(GameContext& context, InstanceID entityID)
	{
		auto& entity = context.m_entityPool.getInstanceRef(entityID);
		return entity.hasState(Physics::PhysicsBody::Type);
	}

	void PhysicsAction::onAddEntity(GameContext& context, InstanceID entityID)
	{
		
	}

	void PhysicsAction::onRemoveEntity(GameContext& context, InstanceID entityID)
	{
		
	}

	void PhysicsAction::onDestroyEntity(GameContext& context, InstanceID entityID)
	{
		auto& physicsEngine = context.m_services.getPhysics();
		auto& entity = context.m_entityPool.getInstanceRef(entityID);
		auto* bodyState = entity.getState<Physics::PhysicsBody>();
		for (uint32_t i = 0; i < bodyState->fixtureCount; ++i)
		{
			physicsEngine.destroyFixture(bodyState->bodyID, bodyState->fixtureID[i]);
			bodyState->fixtureID[i] = 0;
		}
		bodyState->fixtureCount = 0;
		physicsEngine.destroyBody(bodyState->bodyID);
		bodyState->bodyID = 0;
	}
}
