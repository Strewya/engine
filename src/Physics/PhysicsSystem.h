#pragma once
/********************************************
*	class:	PhysicsSystem
*	usage:
********************************************/
/******* C++ headers *******/
#include <algorithm>
#include <vector>
/******* common headers *******/
#include <Box2D/Box2D.h>
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
	class Vec2;

	typedef uint32_t InstanceID;

	class PhysicsSystem : public b2ContactListener
	{
	public:
		PhysicsSystem(); //needed because b2World doesn't have a default ctor

		bool init(const Vec2& gravity, float scalingFactor = 1.0f, b2Draw* debugDraw = nullptr);
		bool shutdown();


		void BeginContact(b2Contact* contact);
		void EndContact(b2Contact* contact);
		void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
		void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

		void setScalingFactor(float scale);
		float getScalingFactor() const;

		Vec2 convert(const b2Vec2& v);
		b2Vec2 convert(const Vec2& v);

		InstanceID createBody(const b2BodyDef& bodyDef);
		b2Body* getBody(InstanceID bodyID) const;
		void destroyBody(InstanceID bodyID);

		InstanceID createFixture(const b2FixtureDef& fixtureDef, InstanceID bodyID);
		b2Fixture* getFixture(InstanceID fixtureID) const;
		void destroyFixture(InstanceID bodyID, uint32_t fixtureID);

		InstanceID createJoint(const b2JointDef& jointDef);
		b2Joint* getJoint(InstanceID jointID) const;
		void destroyJoint(InstanceID jointID);

	private:
		
		float m_scalingFactor;
		float m_scalingFactorInv;

		b2World m_world;

		std::vector<b2Body*> m_bodies;
		std::vector<b2Fixture*> m_fixtures;
		std::vector<b2Joint*> m_joints;

		template<typename C, typename V, typename F> InstanceID store(C& container, V& data, F& cmp);
	};

	template<typename C, typename V, typename F> InstanceID PhysicsSystem::store(C& container, V& data, F& cmp)
	{
		auto it = std::find_if(std::begin(container), std::end(container), cmp);
		InstanceID id = 0;
		if(it == std::end(container))
		{
			id = container.size();
			container.emplace_back(data);
		}
		else
		{
			id = std::distance(std::begin(container), it);
			*it = data;
		}
		return id;
	}
}
