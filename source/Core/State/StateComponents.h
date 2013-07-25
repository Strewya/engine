#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Core/State/State4.h"
#include "Util/Dimensional.h"
	/*** end header inclusion ***/

#define Component(Name) struct Name : public StateType<Name>
namespace Core
{
	Component(Position2d)
	{
		Util::Vec2 position;
	};

	Component(Position3d)
	{
		Util::Vec3 position;
	};

}