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

namespace Core
{
	COMPONENT(Box2dBody)
	{
		b2Body* m_body;
		b2BodyDef m_definition;
	};

	COMPONENT(Box2dFixtures)
	{
		std::vector<std::pair<b2FixtureDef, b2Fixture*>> m_fixtures;
	};
}