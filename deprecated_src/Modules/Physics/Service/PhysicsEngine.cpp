//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Modules/Physics/Service/PhysicsEngine.h>
/******* C++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace Physics
{
	Engine::Engine()
		: m_world(b2Vec2(0, 0)), m_b2ScalingFactor(1), m_b2ScalingFactorInv(1)
	{
		m_bodies.reserve(20);
		m_bodies.reserve(40);
	}

	void Engine::setScalingFactor(float scale)
	{
		m_b2ScalingFactor = scale;
		m_b2ScalingFactorInv = 1 / scale;
	}

	float Engine::getScalingFactor() const
	{
		return m_b2ScalingFactor;
	}

	float Engine::getInverseScalingFactor() const
	{
		return m_b2ScalingFactorInv;
	}

	InstanceID Engine::createBody(const b2BodyDef& bodyDef)
	{
		auto* body = m_world.CreateBody(&bodyDef);
		return store(m_bodies, body, [](const b2Body* ptr){ return ptr == nullptr; }) + 1;
	}

	b2Body* Engine::getBody(InstanceID bodyID) const
	{
		--bodyID;
		assert(bodyID < m_bodies.size() && m_bodies[bodyID] != nullptr);
		return m_bodies[bodyID];
	}

	void Engine::destroyBody(InstanceID bodyID)
	{
		--bodyID;
		assert(bodyID < m_bodies.size() && m_bodies[bodyID] != nullptr);
		m_world.DestroyBody(m_bodies[bodyID]);
		m_bodies[bodyID] = nullptr;
	}

	InstanceID Engine::createFixture(const b2FixtureDef& fixtureDef, InstanceID bodyID)
	{
		auto* body = getBody(bodyID);
		auto* fixture = body->CreateFixture(&fixtureDef);
		return store(m_fixtures, fixture, [](const b2Fixture* ptr){ return ptr == nullptr; }) + 1;
	}

	b2Fixture* Engine::getFixture(InstanceID fixtureID) const
	{
		--fixtureID;
		assert(fixtureID < m_fixtures.size() && m_fixtures[fixtureID] != nullptr);
		return m_fixtures[fixtureID];
	}

	void Engine::destroyFixture(InstanceID bodyID, InstanceID fixtureID)
	{
		--fixtureID;
		assert(fixtureID < m_fixtures.size() && m_fixtures[fixtureID] != nullptr);
		auto* body = getBody(bodyID);
		body->DestroyFixture(m_fixtures[fixtureID]);
		m_fixtures[fixtureID] = nullptr;
	}

	InstanceID Engine::createJoint(const b2JointDef& jointDef)
	{
		auto* joint = m_world.CreateJoint(&jointDef);
		return store(m_joints, joint, [](const b2Joint* ptr){ return ptr == nullptr; }) + 1;
	}

	b2Joint* Engine::getJoint(InstanceID jointID) const
	{
		--jointID;
		assert(jointID < m_joints.size() && m_joints[jointID] != nullptr);
		return m_joints[jointID];
	}

	void Engine::destroyJoint(InstanceID jointID)
	{
		--jointID;
		assert(jointID < m_joints.size() && m_joints[jointID] != nullptr);
		m_world.DestroyJoint(m_joints[jointID]);
		m_joints[jointID] = nullptr;
	}





	CollisionCallback::CollisionCallback()
		: m_beginContactLogic([](b2Contact* c){}),
		m_endContactLogic([](b2Contact* c){}),
		m_preSolveLogic([](b2Contact* c, const b2Manifold* m){}),
		m_postSolveLogic([](b2Contact* c, const b2ContactImpulse* m){})
	{}

	CollisionCallback::CollisionCallback(std::function<void(b2Contact*)> beginContactLogic,
		std::function<void(b2Contact*)> endContactLogic,
		std::function<void(b2Contact*, const b2Manifold*)> preSolveLogic,
		std::function<void(b2Contact*, const b2ContactImpulse*)> postSolveLogic)
		: m_beginContactLogic(beginContactLogic),
		m_endContactLogic(endContactLogic),
		m_preSolveLogic(preSolveLogic),
		m_postSolveLogic(postSolveLogic)
	{}

	void CollisionCallback::setBeginContactLogic(std::function<void(b2Contact*)> beginContactLogic)
	{
		if (beginContactLogic == nullptr)
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
		if (endContactLogic == nullptr)
		{
			m_endContactLogic = [](b2Contact* c){};
		}
		else
		{
			m_endContactLogic = endContactLogic;
		}
	}

	void CollisionCallback::setPreSolveLogic(std::function<void(b2Contact*, const b2Manifold*)> preSolveLogic)
	{
		if (preSolveLogic == nullptr)
		{
			m_preSolveLogic = [](b2Contact* c, const b2Manifold* m){};
		}
		else
		{
			m_preSolveLogic = preSolveLogic;
		}
	}

	void CollisionCallback::setPostSolveLogic(std::function<void(b2Contact*, const b2ContactImpulse*)> postSolveLogic)
	{
		if (postSolveLogic == nullptr)
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
