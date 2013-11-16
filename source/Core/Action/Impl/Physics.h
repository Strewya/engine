#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Box2D/Box2D.h"
#include "Core/Action/Action.h"
#include "Services/Graphics/b2DebugDraw.h"
	/*** end header inclusion ***/
	
namespace Core
{
	class Physics2d : public AtomicAction<Physics2d>
	{
	public:
		Physics2d();
		Physics2d(const Util::Vec2& gravity, float scalingFactor);

		void init(GameContext& context);
		void processMessage(GameContext& context, uint32_t msg, InstanceID entity);
		void update(GameContext& context);
		void render(GameContext& context, uint64_t interpolationTime);

		void onDestroyEntity(GameContext& context, InstanceID entity);

		b2World m_physicsWorld;
		float m_b2ScalingFactor;
		float m_b2ScalingFactorInv;

		Graphics::b2DebugDraw m_debug;

	};



	class CollisionCallback : public b2ContactListener
	{
	public:
		CollisionCallback();
		CollisionCallback(std::function<void(b2Contact*)> beginContactLogic,
			std::function<void(b2Contact*)> endContactLogic,
			std::function<void(b2Contact*,const b2Manifold*)> preSolveLogic,
			std::function<void(b2Contact*,const b2ContactImpulse*)> postSolveLogic);

		void setBeginContactLogic(std::function<void(b2Contact*)> beginContactLogic);
		void setEndContactLogic(std::function<void(b2Contact*)> endContactLogic);
		void setPreSolveLogic(std::function<void(b2Contact*,const b2Manifold*)> preSolveLogic);
		void setPostSolveLogic(std::function<void(b2Contact*,const b2ContactImpulse*)> postSolveLogic);

		void BeginContact(b2Contact* contact);
		void EndContact(b2Contact* contact);
		void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
		void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

	private:
		std::function<void(b2Contact*)> m_beginContactLogic;
		std::function<void(b2Contact*)> m_endContactLogic;
		std::function<void(b2Contact*,const b2Manifold*)> m_preSolveLogic;
		std::function<void(b2Contact*,const b2ContactImpulse*)> m_postSolveLogic;
	};
}