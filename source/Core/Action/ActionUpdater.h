#ifndef CORE_ACTION_ACTIONUPDATER_H_
#define CORE_ACTION_ACTIONUPDATER_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <list>
#include <set>
#include <unordered_map>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Core/Action/ActionLogic.h"
	/*** end header inclusion ***/

namespace Core
{
	class Action;
	class ServiceLocator;

	class ActionUpdater
	{
	private:
		std::unordered_map<ActionLogic, std::set<Action*>> _activeActions;
		std::list<std::pair<ActionLogic, Action*>> _actionsForDeletion;
		std::vector<ActionLogic> _actionQueue;

		void PurgeActions();

	public:
		void Update(double dt, GameContext& context);

		bool EnqueueActionLogic(ActionLogic logic);
		bool ActivateAction(ActionLogic logic, Action* action);
		bool DeactivateAction(ActionLogic logic, Action* action);
	};
}

#endif //CORE_ACTION_ACTIONUPDATER_H_