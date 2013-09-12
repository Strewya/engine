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
	class ActionRegistry
	{
	public:
		typedef std::unordered_map<InstanceID , ActionUptr> ActionStorage_t;

		ActionRef addAction(ActionUptr action);
		ActionRef getAction(InstanceID actionId);

		ActionStorage_t::iterator begin();
		ActionStorage_t::iterator end();

	protected:
	private:
		
		ActionStorage_t m_actions;
	};
}