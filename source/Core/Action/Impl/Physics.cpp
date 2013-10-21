//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Core/Action/Impl/Physics.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Core/State/SharedComponents.h"
#include "Engine/GameContext.h"
	/*** end headers ***/
	
namespace Core
{
	void Physics::frameUpdate(float dt)
	{
		//do anything required before the step

		//step the physics
		int32_t velocityIterations = 8;
		int32_t positionIterations = 3;
		m_context.physicsWorld.Step(dt, velocityIterations, positionIterations);

		//do any post step updates, like updating the velocity states an such
		for(auto body = m_context.physicsWorld.GetBodyList(); body != nullptr; body = body->GetNext())
		{
			Entity* e = static_cast<Entity*>(body->GetUserData());
			if(e->hasState(Position2d::Type))
			{
				auto* pos = e->getState<Position2d>();
				pos->data.set(body->GetPosition().x*m_context.b2ScalingFactor, body->GetPosition().y*m_context.b2ScalingFactor);
			}
		}
	}

	void Physics::init()
	{
		m_context.physicsWorld.SetGravity(b2Vec2(0, -9));
		m_context.physicsWorld.SetAllowSleeping(true);

		m_context.entityPool.registerDestructionCallback([&](Entity& e)
		{
			if(e.hasState(PhysicalBody::Type))
			{
				auto* b = e.getState<PhysicalBody>();
				m_context.physicsWorld.DestroyBody(b->data);
				b->data = nullptr;
			}
		});

		
	}

	bool Physics::validateEntity(Entity& entity) const
	{
		return
			entity.hasState(PhysicalBody::Type) &&
			entity.hasState(Position2d::Type);
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