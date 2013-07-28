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

#define Component(Name) struct Name : public StateType<Name>
namespace Core
{
	/**
		RULES FOR WRITING COMPONENTS:
		1. If the component has only one property in it, the name of the property must be 'value'
		2. Do NOT use Components inside other Components
		...

	*/
	Component(Position2d)
	{
		Util::Vec2 value;
	};

	Component(Position3d)
	{
		Util::Vec3 value;
	};

	
}