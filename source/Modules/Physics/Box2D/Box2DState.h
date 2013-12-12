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
	COMPONENT(Box2dBodyDef)
	{
		b2BodyDef m_def;
	};

	struct FixtureData
	{
		b2FixtureDef m_def;
		b2PolygonShape m_polygon;
		b2CircleShape m_circle;
		b2EdgeShape m_edge;
		b2ChainShape m_chain;
	};

	COMPONENT(Box2dFixtures)
	{
		std::vector<FixtureData> m_fixtures;
	};

	struct JointData
	{
		InstanceID m_bodyA;
		InstanceID m_bodyB;
        std::function<void(JointData&,b2Body*,b2Body*)> m_initDefinition;
        b2PrismaticJointDef m_prismatic;
	};

	COMPONENT(Box2dJoint)
	{
		std::vector<JointData> m_joints;
	};
}
