//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Core/Action/Impl/Physics.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Core/State/SharedComponents.h"
#include "Engine/GameContext.h"
#include "Engine/ServiceLocator.h"
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
		AtomicAction::init(context);
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

	}

	void Physics2d::processMessage(GameContext& context, uint32_t msg, InstanceID entity)
	{
	}

	void Physics2d::update(GameContext& context)
	{
		//do anything required before the step
		
		//step the physics
		const int32_t velocityIterations = 8;
		const int32_t positionIterations = 3;
		m_physicsWorld.Step(context.m_timer.getDeltaTime(), velocityIterations, positionIterations);

		//do any post step updates, like updating the velocity states an such

	}

	void Physics2d::render(GameContext& context, uint64_t interpolationTime)
	{
		m_physicsWorld.DrawDebugData();
	}

	void Physics2d::onDestroyEntity(GameContext& context, InstanceID entity)
	{
		Entity& e = context.m_entityPool.getInstanceRef(entity);
		if(e.hasState(PhysicalBody::Type))
		{
			auto* state = e.getState<PhysicalBody>();
			m_physicsWorld.DestroyBody(state->m_body);
			state->m_body = nullptr;
		}
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