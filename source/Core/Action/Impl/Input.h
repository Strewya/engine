#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <functional>
#include <unordered_map>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Core/Action/Action.h"
	/*** end header inclusion ***/

namespace Input { class Event; }

namespace Core
{
	SYSTEM(AInput)
	{
	public:
		SYSTEM_CTOR(AInput);

	protected:
		void onUpdate(float dt);
		
	private:
		
	};
}