#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include <Engine/Defines.h>
	/*** extra headers if needed (alphabetically ordered) ***/
#include <Box2D/Box2D.h>
#include <Core/Action/Action.h>
#include <Modules/Rendering/Service/b2DebugDraw.h>
	/*** end header inclusion ***/
	
namespace Physics
{
	class Physics2d : public Core::AtomicAction<Physics2d>
	{
	public:
		typedef std::function<void(b2Body*)> BodyDeleter_t;
		typedef std::unique_ptr<b2Body, BodyDeleter_t> BodyUptr_t;

	public:
		Physics2d();
		Physics2d(const Util::Vec2& gravity, float scalingFactor);

		void init(Core::GameContext& context);
		void processMessage(Core::GameContext& context, uint32_t msg, InstanceID entity);
		void update(Core::GameContext& context);
		void render(Core::GameContext& context, uint64_t interpolationTime);

		bool validateEntity(Core::GameContext& context, InstanceID entity);
		void onAddEntity(Core::GameContext& context, InstanceID entity);
		void onRemoveEntity(Core::GameContext& context, InstanceID entity);
		void onDestroyEntity(Core::GameContext& context, InstanceID entity);

		BodyUptr_t createBody(const b2BodyDef& definition);

	private:
		
		float m_b2ScalingFactor;
		float m_b2ScalingFactorInv;

		b2World m_physicsWorld;
		std::unordered_map<InstanceID, BodyUptr_t> m_bodies;

		Graphics::b2DebugDraw m_debug;
		
		uint32_t m_msgCreateBody;
		uint32_t m_msgCreateFixtures;
		uint32_t m_msgCreateJoint;
		uint32_t m_msgSetPosition;
		uint32_t m_msgApplyImpulse;
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