#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <unordered_map>
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Core
{
	class ActionIndex
	{
	public:
		ActionRef addActionToIndex(ActionUptr action);
		ActionRef getActionFromIndex(InstanceID actionId);

	protected:
	private:
		typedef std::unordered_map<InstanceID , ActionUptr> ActionStorage_t;
		ActionStorage_t _actions;
	};
}