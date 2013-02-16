#ifndef CORE_ACTION_ACTIONMAP_H_
#define CORE_ACTION_ACTIONMAP_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Core/Action/Action.h"
	/*** end header inclusion ***/

namespace Core
{
	class Actionmap
	{
	private:
		typedef std::unordered_map<String, Action> ActionCache;
		ActionCache _cache;

	public:
		
		
	};
}

#endif //CORE_ACTION_ACTIONMAP_H_