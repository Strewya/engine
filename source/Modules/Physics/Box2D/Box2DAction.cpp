//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include <stdafx.h>
	/*** personal header ***/
#include <Core/Action/Impl/Physics.h>
	/*** C++ headers ***/
	/*** extra headers ***/
#include <Core/State/GeneralComponents.h>
#include <Core/State/Impl/Box2dPhysics.h>
#include <Engine/GameContext.h>
#include <Engine/ServiceLocator.h>
	/*** end headers ***/
	
namespace Core
{
	Util::Vec2 convert(const b2Vec2& v, float scaling)
	{
		return Util::Vec2(v.x*scaling, v.y*scaling);
	}

	b2Vec2 convert(const Util::Vec2& v, float scalingInverse)
	{
		return b2Vec2(v.x*scalingInverse, v.y*scalingInverse);
	}

	Physics2d::Physics2d()
		: m_physicsWorld(b2Vec2(0,-9)),
		  m_b2ScalingFactor(1),
		  m_b2ScalingFactorInv(1/m_b2ScalingFactor)
	{
	}

	Physics2d::Physics2d(const Util::Vec2& gravity, float scaling)
		: m_physicsWorld(convert(gravity, scaling)),
		  m_b2ScalingFactor(scaling),
		  m_b2ScalingFactorInv(1/m_b2ScalingFactor)
	{
	}

	void Physics2d::init(GameContext& context)
	{
		m_physicsWorld.SetAllowSleeping(true);

		uint32_t flags = 0;
		flags += Graphics::b2DebugDraw::e_shapeBit;
		flags += Graphics::b2DebugDraw::e_centerOfMassBit;
		flags += Graphics::b2DebugDraw::e_jointBit;
		flags += Graphics::b2DebugDraw::e_pairBit;
		//flags += Graphics::b2DebugDraw::e_aabbBit;
		m_debug.SetFlags(flags);

		m_debug.setLengthScale(m_b2ScalingFactor);
		m_debug.setRenderer(context.m_services.getGraphics());

		m_physicsWorld.SetDebugDraw(&m_debug);

		m_msgCreateBody = context.m_messenger.encode("create_body");
		m_msgCreateFixtures = context.m_messenger.encode("create_fixtures");
		m_msgCreateJoint = context.m_messenger.encode("create_joints");
		m_msgSetPosition = context.m_messenger.encode("set_position");
		m_msgApplyImpulse = context.m_messenger.encode("apply_impulse");
	}

	void Physics2d::processMessage(GameContext& context, uint32_t msg, InstanceID entity)
	{
		if(!context.m_entityPool.isAlive(entity)) return;
		Entity& e = context.m_entityPool.getInstanceRef(entity);
		
		if(msg == m_msgCreateBody)
		{
			auto* bodyDef = e.getState<Box2dBodyDef>();
			auto* position = e.getState<Position2d>();
			if(bodyDef != nullptr && position != nullptr)
			{
				bodyDef->m_def.position = convert(position->m_position, m_b2ScalingFactor);
				auto pair = m_bodies.emplace(entity, createBody(bodyDef->m_def));
				pair.first->second->SetActive(false);
				e.removeState(bodyDef->uid);
				bodyDef = nullptr;
			}
		}
		else if(msg == m_msgCreateFixtures)
		{
			auto it = m_bodies.find(entity);
			if(it == m_bodies.end()) return;
			b2Body& body = *it->second;

			auto* fixtures = e.getState<Box2dFixtures>();
			
			if(fixtures != nullptr)
			{
				for(auto& fixture : fixtures->m_fixtures)
				{
					body.CreateFixture(&fixture.m_def);
				}
				e.removeState(fixtures->uid);
				fixtures = nullptr;
			}
		}
		else if(msg == m_msgCreateJoint)
		{
			auto it = m_bodies.find(entity);
			if(it == m_bodies.end()) return;
			b2Body& body = *it->second;

			auto* joints = e.getState<Core::Box2dJoint>();
			if(joints != nullptr)
			{
				for(auto& joint : joints->m_joints)
				{
					if(m_bodies.count(joint.m_bodyA) != 0 && m_bodies.count(joint.m_bodyB) != 0)
                    {
                        joint.m_initDefinition(joint, m_bodies[joint.m_bodyA].get(), m_bodies[joint.m_bodyB].get());
                        m_physicsWorld.CreateJoint(&joint.m_prismatic);
					}
				}
			}
		}
		else if(msg == m_msgSetPosition)
		{
			auto it = m_bodies.find(entity);
			if(it == m_bodies.end()) return;
			b2Body& body = *it->second;

			auto* position = e.getState<Position2d>();
			if(position != nullptr)
			{
				body.SetTransform(convert(position->m_position, m_b2ScalingFactorInv), 0);
			}
		}
		else if(msg == m_msgApplyImpulse)
		{
			auto it = m_bodies.find(entity);
			if(it == m_bodies.end()) return;
			b2Body& body = *it->second;

			auto* impulse = e.getState<Impulse2d>();
			if(impulse != nullptr)
			{
				body.ApplyLinearImpulse(convert(impulse->m_impulse, m_b2ScalingFactorInv), body.GetWorldCenter());
			}
		}
	}

	void Physics2d::update(GameContext& context)
	{
		//do anything required before the step
		
		//step the physics
		const int32_t velocityIterations = 8;
		const int32_t positionIterations = 3;
		m_physicsWorld.Step(context.m_timer.getDeltaTime(), velocityIterations, positionIterations);

		//do any post step updates, like updating the velocity states an such
		for(auto eid : m_entities)
		{
			Entity& e = context.m_entityPool.getInstanceRef(eid);
			auto* position = e.getState<Position2d>();
			auto* body = m_bodies[eid].get();
			position->m_position.set(convert(body->GetPosition(), m_b2ScalingFactor));
		}
	}

	void Physics2d::render(GameContext& context, uint64_t interpolationTime)
	{
		m_physicsWorld.DrawDebugData();
	}

	bool Physics2d::validateEntity(GameContext& context, InstanceID entity)
	{
		Entity& e = context.m_entityPool.getInstanceRef(entity);
		return m_bodies.count(entity) != 0 &&
			e.hasState(Position2d::Type);
	}

	void Physics2d::onAddEntity(GameContext& context, InstanceID entity)
	{
		m_bodies[entity]->SetActive(true);
	}

	void Physics2d::onRemoveEntity(GameContext& context, InstanceID entity)
	{
		m_bodies[entity]->SetActive(false);
	}

	void Physics2d::onDestroyEntity(GameContext& context, InstanceID entity)
	{
		m_bodies.erase(entity);
	}

	auto Physics2d::createBody(const b2BodyDef& definition) -> BodyUptr_t
	{
		return BodyUptr_t(m_physicsWorld.CreateBody(&definition), [&](b2Body* b){m_physicsWorld.DestroyBody(b);});
	}





	CollisionCallback::CollisionCallback()
		: m_beginContactLogic([](b2Contact* c){}),
		m_endContactLogic([](b2Contact* c){}),
		m_preSolveLogic([](b2Contact* c, const b2Manifold* m){}),
		m_postSolveLogic([](b2Contact* c, const b2ContactImpulse* m){})
	{}

	CollisionCallback::CollisionCallback(std::function<void(b2Contact*)> beginContactLogic,
			std::function<void(b2Contact*)> endContactLogic,
			std::function<void(b2Contact*,const b2Manifold*)> preSolveLogic,
			std::function<void(b2Contact*,const b2ContactImpulse*)> postSolveLogic)
		: m_beginContactLogic(beginContactLogic),
		m_endContactLogic(endContactLogic),
		m_preSolveLogic(preSolveLogic),
		m_postSolveLogic(postSolveLogic)
	{}

	void CollisionCallback::setBeginContactLogic(std::function<void(b2Contact*)> beginContactLogic)
	{
		if(beginContactLogic == nullptr)
		{
			m_beginContactLogic = [](b2Contact* c){};
		}
		else
		{
			m_beginContactLogic = beginContactLogic;
		}
	}

	void CollisionCallback::setEndContactLogic(std::function<void(b2Contact*)> endContactLogic)
	{
		if(endContactLogic == nullptr)
		{
			m_endContactLogic = [](b2Contact* c){};
		}
		else
		{
			m_endContactLogic = endContactLogic;
		}
	}

	void CollisionCallback::setPreSolveLogic(std::function<void(b2Contact*,const b2Manifold*)> preSolveLogic)
	{
		if(preSolveLogic == nullptr)
		{
			m_preSolveLogic = [](b2Contact* c, const b2Manifold* m){};
		}
		else
		{
			m_preSolveLogic = preSolveLogic;
		}
	}

	void CollisionCallback::setPostSolveLogic(std::function<void(b2Contact*,const b2ContactImpulse*)> postSolveLogic)
	{
		if(postSolveLogic == nullptr)
		{
			m_postSolveLogic = [](b2Contact* c, const b2ContactImpulse* m){};
		}
		else
		{
			m_postSolveLogic = postSolveLogic;
		}
	}

	void CollisionCallback::BeginContact(b2Contact* contact)
	{
		m_beginContactLogic(contact);
	}

	void CollisionCallback::EndContact(b2Contact* contact)
	{
		m_endContactLogic(contact);
	}

	void CollisionCallback::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	{
		m_preSolveLogic(contact, oldManifold);
	}

	void CollisionCallback::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
	{
		m_postSolveLogic(contact, impulse);
	}
























}
