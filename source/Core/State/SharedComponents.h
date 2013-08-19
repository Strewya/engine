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
		1. If the component has only one property in it, the name of the property must be 'data'
		2. Do NOT use Components inside other Components
		...

	*/
	COMPONENT(Position2d)
	{
		Util::Vec2 data;
	};

	COMPONENT(Position3d)
	{
		Util::Vec3 data;
	};

	COMPONENT(PhysicalBody)
	{
		b2Body* data;
	};
}
