#pragma once
/********************************************
*	class:	PhysicsEngine
*	usage:
********************************************/
/******* common header *******/
#include <Engine/Defines.h>
#include <Engine/FwdDecl.h>
#include <vector>
/******* extra headers if needed *******/
#include <Box2D/Box2D.h>
/******* end header inclusion *******/

namespace Physics
{
	class Engine
	{	
	public:
		Engine();

		void setScalingFactor(float scale);
		float getScalingFactor() const;
		float getInverseScalingFactor() const;

		InstanceID createBody(const b2BodyDef& bodyDef);
		b2Body* getBody(InstanceID bodyID) const;
		void destroyBody(InstanceID bodyID);

		InstanceID createFixture(const b2FixtureDef& fixtureDef, InstanceID bodyID);
		b2Fixture* getFixture(InstanceID fixtureID) const;
		void destroyFixture(InstanceID bodyID, InstanceID fixtureID);

		InstanceID createJoint(const b2JointDef& jointDef);
		b2Joint* getJoint(InstanceID jointID) const;
		void destroyJoint(InstanceID jointID);

		b2World m_world;
		

	private:
		float m_b2ScalingFactor;
		float m_b2ScalingFactorInv;

		std::vector<b2Body*> m_bodies;
		std::vector<b2Fixture*> m_fixtures;
		std::vector<b2Joint*> m_joints;

		template<typename C, typename V, typename F> InstanceID store(C& container, V data, F cmp)
		{
			auto it = std::find_if(std::begin(container), std::end(container), cmp);
			InstanceID id = 0;
			if (it == std::end(container))
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
	};




	class CollisionCallback : public b2ContactListener
	{
	public:
		CollisionCallback();
		CollisionCallback(std::function<void(b2Contact*)> beginContactLogic,
			std::function<void(b2Contact*)> endContactLogic,
			std::function<void(b2Contact*, const b2Manifold*)> preSolveLogic,
			std::function<void(b2Contact*, const b2ContactImpulse*)> postSolveLogic);

		void setBeginContactLogic(std::function<void(b2Contact*)> beginContactLogic);
		void setEndContactLogic(std::function<void(b2Contact*)> endContactLogic);
		void setPreSolveLogic(std::function<void(b2Contact*, const b2Manifold*)> preSolveLogic);
		void setPostSolveLogic(std::function<void(b2Contact*, const b2ContactImpulse*)> postSolveLogic);

		void BeginContact(b2Contact* contact);
		void EndContact(b2Contact* contact);
		void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
		void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

	private:
		std::function<void(b2Contact*)> m_beginContactLogic;
		std::function<void(b2Contact*)> m_endContactLogic;
		std::function<void(b2Contact*, const b2Manifold*)> m_preSolveLogic;
		std::function<void(b2Contact*, const b2ContactImpulse*)> m_postSolveLogic;
	};



	inline Util::Vec2 convert(const b2Vec2& v, float scaling)
	{
		return Util::Vec2(v.x*scaling, v.y*scaling);
	}

	inline b2Vec2 convert(const Util::Vec2& v, float scalingInverse)
	{
		return b2Vec2(v.x*scalingInverse, v.y*scalingInverse);
	}
}
