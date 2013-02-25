#ifndef CORE_ACTION_ACTION_UPDATER_H_
#define CORE_ACTION_ACTION_UPDATER_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
#include <set>
#include <unordered_map>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Core/Action/ActionLogic.h"
	/*** end header inclusion ***/

namespace Core
{
	class Entity;
	class ServiceLocator;

	class ActionUpdater
	{
	private:
		std::unordered_map<ActionLogic, std::set<Entity*>> _activeActions;
		std::vector<ActionLogic> _actionQueue;

	public:
		void Update(float dt, ServiceLocator& services);

		bool EnqueueAction(ActionLogic logic);
		bool ActivateEntity(ActionLogic logic, Entity* entity);
	};
}

#endif //CORE_ACTION_ACTION_UPDATER_H_