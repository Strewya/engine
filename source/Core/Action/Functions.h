#ifndef CORE_ACTION_FUNCTIONS_H_
#define CORE_ACTION_FUNCTIONS_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
#include <set>
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Core
{
	class Entity;
}

typedef void(*Func)(const std::set<Core::Entity*>& ents);

namespace Core
{
	void Render(const std::set<Core::Entity*>& ents);
}

#endif //CORE_ACTION_FUNCTIONS_H_