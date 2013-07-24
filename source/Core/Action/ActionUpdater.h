#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <list>
	/*** extra headers if needed (alphabetically ordered) ***/
//#include "Core/Action/Action.h"
	/*** end header inclusion ***/

namespace Core
{
	class ActionUpdater
	{
	public:
		void update(float dt, GameContext& context);
	
		void addAction(ActionRef action);

	private:
		
		std::list<ActionRptr> _priorityQueue;
	};
}