#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Core/State/State4.h"
#include "Core/State/StateComponents.h"
	/*** end header inclusion ***/

namespace Core
{
	typedef std::unique_ptr<BaseState> StateUptr;
	typedef BaseState* StateRptr;
	typedef BaseState& StateRef;
}