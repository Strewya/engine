#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include <Engine/Defines.h>
#include <unordered_map>
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Core
{
	class ActionRegistry
	{
	public:
		typedef std::unordered_multimap<InstanceID, ActionUptr> ActionStorage_t;

		ActionRef addAction(ActionUptr action);

		
	protected:
	private:
		ActionStorage_t m_actions;
	};
}