//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Physics/PhysicsSystem.h>
/******* C++ headers *******/
#include <iostream>
/******* extra headers *******/
#include <Util/Dimensional.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	PhysicsSystem::PhysicsSystem()
		: m_world(b2Vec2(0, -9))
	{
		m_bodies.reserve(20);
		m_fixtures.reserve(40);
		m_joints.reserve(20);
	}

	bool PhysicsSystem::init(const Vec2& gravity, b2Draw* debugDraw)
	{
		bool status = true;

		m_world.SetGravity(convert(gravity));
		if(debugDraw != nullptr)
			m_world.SetDebugDraw(debugDraw);
		m_world.SetContactListener(this);

		DEBUG_INFO("PhysicsSystem init ", status ? "OK" : "FAIL");
		return status;
	}

	bool PhysicsSystem::shutdown()
	{
		bool status = true;

		for(auto* joint : m_joints)
		{
			m_world.DestroyJoint(joint);
		}
		
		for(auto* body : m_bodies)
		{
			m_world.DestroyBody(body);
		}

		m_joints.clear();
		m_fixtures.clear();
		m_bodies.clear();

		DEBUG_INFO("PhysicsSystem shutdown ", status ? "OK" : "FAIL");
		return status;
	}

	void PhysicsSystem::update(float dt)
	{
		const int32_t velocityIterations = 8;
		const int32_t positionIterations = 3;
		m_world.Step(dt, velocityIterations, positionIterations);
	}

	void PhysicsSystem::draw()
	{
		m_world.DrawDebugData();
	}

	InstanceID PhysicsSystem::createBody(const b2BodyDef& bodyDef)
	{
		auto* body = m_world.CreateBody(&bodyDef);
		return store(m_bodies, body, [](const b2Body* ptr){ return ptr == nullptr; }) + 1;
	}

	b2Body* PhysicsSystem::getBody(InstanceID bodyID) const
	{
		--bodyID;
		assert(bodyID < m_bodies.size() && m_bodies[bodyID] != nullptr);
		return m_bodies[bodyID];
	}

	void PhysicsSystem::destroyBody(InstanceID bodyID)
	{
		--bodyID;
		assert(bodyID < m_bodies.size() && m_bodies[bodyID] != nullptr);
		m_world.DestroyBody(m_bodies[bodyID]);
		m_bodies[bodyID] = nullptr;
	}

	InstanceID PhysicsSystem::createFixture(const b2FixtureDef& fixtureDef, InstanceID bodyID)
	{
		auto* body = getBody(bodyID);
		auto* fixture = body->CreateFixture(&fixtureDef);
		return store(m_fixtures, fixture, [](const b2Fixture* ptr){ return ptr == nullptr; }) + 1;
	}

	b2Fixture* PhysicsSystem::getFixture(InstanceID fixtureID) const
	{
		--fixtureID;
		assert(fixtureID < m_fixtures.size() && m_fixtures[fixtureID] != nullptr);
		return m_fixtures[fixtureID];
	}

	void PhysicsSystem::destroyFixture(InstanceID bodyID, InstanceID fixtureID)
	{
		--fixtureID;
		assert(fixtureID < m_fixtures.size() && m_fixtures[fixtureID] != nullptr);
		auto* body = getBody(bodyID);
		body->DestroyFixture(m_fixtures[fixtureID]);
		m_fixtures[fixtureID] = nullptr;
	}

	InstanceID PhysicsSystem::createJoint(const b2JointDef& jointDef)
	{
		auto* joint = m_world.CreateJoint(&jointDef);
		return store(m_joints, joint, [](const b2Joint* ptr){ return ptr == nullptr; }) + 1;
	}

	b2Joint* PhysicsSystem::getJoint(InstanceID jointID) const
	{
		--jointID;
		assert(jointID < m_joints.size() && m_joints[jointID] != nullptr);
		return m_joints[jointID];
	}

	void PhysicsSystem::destroyJoint(InstanceID jointID)
	{
		--jointID;
		assert(jointID < m_joints.size() && m_joints[jointID] != nullptr);
		m_world.DestroyJoint(m_joints[jointID]);
		m_joints[jointID] = nullptr;
	}

	void PhysicsSystem::addBeginContactListener(std::function<void(b2Contact*)> f)
	{
		m_beginContactListeners.emplace_back(f);
	}

	void PhysicsSystem::addEndContactListener(std::function<void(b2Contact*)> f)
	{
		m_endContactListeners.emplace_back(f);
	}

	void PhysicsSystem::BeginContact(b2Contact* contact)
	{
		for(auto& f : m_beginContactListeners)
		{
			f(contact);
		}	
	}

	void PhysicsSystem::EndContact(b2Contact* contact)
	{
		for(auto& f : m_endContactListeners)
		{
			f(contact);
		}
	}

	void PhysicsSystem::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	{
		
	}

	void PhysicsSystem::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
	{
		
	}
}
