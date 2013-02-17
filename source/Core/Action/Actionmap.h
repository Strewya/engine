#ifndef CORE_ACTION_ACTIONMAP_H_
#define CORE_ACTION_ACTIONMAP_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
#include <unordered_map>
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
		Action& Insert(const char* name, const Action& action);
		Action& Insert(const String& name, const Action& action);

		Action& Get(const char* name);
		Action& Get(const String& name);
	};
}

#endif //CORE_ACTION_ACTIONMAP_H_