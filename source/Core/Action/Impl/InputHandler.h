#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <functional>
#include <list>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Core/Action/Action.h"
	/*** end header inclusion ***/

namespace Core
{
	SYSTEM(InputHandler)
	{
	public:
		SYSTEM_CTOR(InputHandler);
		
	protected:
		void frameUpdate(float dt);
		bool validateEntity(Entity& id) const;
	};
}