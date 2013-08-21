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
	bool APhysics::onUpdate(float dt)
	{
		m_context.physicsWorld.Step(dt, 6, 3);

		return true;
	}

	void APhysics::init()
	{
		m_timer.setUpdatePeriod(1.0f/60);
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


}