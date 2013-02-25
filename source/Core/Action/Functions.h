#ifndef CORE_ACTION_FUNCTIONS_H_
#define CORE_ACTION_FUNCTIONS_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
#include <memory>
#include <set>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "ActionLogic.h"
	/*** end header inclusion ***/

namespace Core
{
	void Render(float dt, /*const ResourceLocator& resources,*/ const Core::ServiceLocator& services, std::set<Core::Entity*>& ents);
}

#endif //CORE_ACTION_FUNCTIONS_H_