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
	/*** end header inclusion ***/
	
namespace Core
{
	SYSTEM(Physics2d)
	{
	public:
		SYSTEM_CTOR(Physics2d);
		

	protected:
		void init();
		void frameUpdate(float dt);
		bool validateEntity(Entity& id) const;

		void doMove(const Intent& i);
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