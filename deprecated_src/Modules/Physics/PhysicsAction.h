#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include <Engine/Defines.h>
	/*** extra headers if needed (alphabetically ordered) ***/
#include <Core/Action/Action.h>
#include <Modules/Physics/Service/PhysicsEngine.h>
#include <Modules/Graphics/Service/b2DebugDraw.h>
	/*** end header inclusion ***/
	
namespace Physics
{
	class PhysicsAction : public Core::AtomicAction<PhysicsAction>
	{
	public:
		void init(Core::GameContext& context);
		void processMessage(Core::GameContext& context, uint32_t msg, InstanceID entity);
		void update(Core::GameContext& context);
		void render(Core::GameContext& context, uint64_t interpolationTime);

		bool validateEntity(Core::GameContext& context, InstanceID entity);
		void onAddEntity(Core::GameContext& context, InstanceID entity);
		void onRemoveEntity(Core::GameContext& context, InstanceID entity);
		void onDestroyEntity(Core::GameContext& context, InstanceID entity);

	private:
		
		Graphics::b2DebugDraw m_debug;
		
		uint32_t m_msgCreateBody;
		uint32_t m_msgCreateFixtures;
		uint32_t m_msgCreateJoint;
		uint32_t m_msgSetPosition;
		uint32_t m_msgApplyImpulse;
	};
}