#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Box2D/Box2D.h"
#include "Core/State/State.h"
#include "Util/Dimensional.h"
	/*** end header inclusion ***/

namespace Core
{
	/**
		RULES FOR WRITING COMPONENTS:
		1. Do NOT use Components inside other Components
		

	*/
	COMPONENT(Position2d)
	{
		Util::Vec2 m_position;
	};

	COMPONENT(PhysicalBody)
	{
		b2Body* m_body;
	};

	COMPONENT(Movement2d)
	{
		Util::Vec2 m_curVelocity;
		Util::Vec2 m_maxVelocity;
		Util::Vec2 m_acceleration;
	};
}
