#pragma once
/********************************************
	class:	Box2dPhysics
	usage:	
********************************************/
	/*** common header ***/
#include <Engine/Defines.h>
#include <Engine/FwdDecl.h>
#include <vector>
	/*** extra headers if needed ***/
#include <Box2D/Box2D.h>
#include <Core/State/State.h>
	/*** end header inclusion ***/

namespace Physics
{
	COMPONENT(PhysicsBody)
	{
		static const int32_t MAX_FIXTURES = 10;
		InstanceID bodyID;
		uint32_t fixtureCount;
		InstanceID fixtureID[MAX_FIXTURES];
	};

}
