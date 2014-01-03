#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include <Engine/Defines.h>
	/*** extra headers if needed (alphabetically ordered) ***/
#include <Core/State/State.h>
#include <Util/Dimensional.h>
	/*** end header inclusion ***/

namespace Core
{
	/**
		RULES FOR WRITING COMPONENTS:
		
	*/

	COMPONENT(Position2d)
	{
		Util::Vec2 m_position;
	};

	COMPONENT(Velocity2d)
	{
		Util::Vec2 m_curVelocity;
		Util::Vec2 m_maxVelocity;
	};

	COMPONENT(Impulse2d)
	{
		Util::Vec2 m_impulse;
	};

	COMPONENT(Acceleration2d)
	{
		Util::Vec2 m_curAcceleration;
		Util::Vec2 m_maxAcceleration;
	};
}