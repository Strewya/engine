#ifndef CORE_ACTION_ACTION_LOGIC_H_
#define CORE_ACTION_ACTION_LOGIC_H_
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
	class ServiceLocator;
}

typedef void(*ActionLogic)(float dt, /*const ResourceLocator& resources,*/ const Core::ServiceLocator& services, std::set<Core::Entity*>& ents);

#endif //CORE_ACTION_ACTION_LOGIC_H_