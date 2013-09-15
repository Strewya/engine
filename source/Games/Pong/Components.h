#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <functional>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Core/State/SharedComponents.h"
	/*** end header inclusion ***/

namespace Pong
{
	/**
		RULES FOR WRITING COMPONENTS:
		1. If the component has only one property in it, the name of the property must be 'value'
		2. Do NOT use Components inside other Components
		...

	*/

	enum class Intent
	{
		LeftMoveUp = 0,
		LeftMoveDown,
		RightMoveUp,
		RightMoveDown,
		ResetBoard,
		INTENT_COUNT,
		NotMapped,
	};

	COMPONENT(AvailableIntents)
	{
		std::function<void(void)> value[Intent::INTENT_COUNT];
	};
	
}