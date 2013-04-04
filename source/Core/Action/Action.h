#ifndef CORE_ACTION_ACTION_H_
#define CORE_ACTION_ACTION_H_
/********************************************
	class:	Action
	usage:	Each Entity class has instances of this class contained within.
			Each instance of Action has a function pointer as it's own member.
			The use pattern for the Action is:
				* Action is created and the function pointer is set for it
				* Action is added to an Entity's list of possible actions
				* Action is started, and it's member 'active' is set to true
					- this is a mere simplification, and is used instead of
					  moving the action from an 'inactive' to an 'active' list
				* when started, the Action's owning Entity is added to a list 
				  of all Entities that are being processed for that function pointer
				* an Action queue is set so that it invokes the function pointers in a certain order
				* when an action is invoked, it's passed the list of entities to process, 
				  the resource locator and the subsystem locator objects
			Updating the action:
				* the Engine calls context.Update(dt);
				* the Context invokes Update on the ActionUpdater object
				* the ActionUpdater object has a priority sorted list of pairs <function pointer, entities to process>
				* for each pair the function pointer is invoked by passing in the list of entities, the subsystems locator and the resources locator
				* 
				* 
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Core/Action/ActionLogic.h"
	/*** end header inclusion ***/

namespace Core
{
	class ActionUpdater;
	class Entity;

	class Action
	{
	private:
		static ActionUpdater* _actionUpdater;

		ActionLogic _actionLogic;
		bool _active;
		Entity* _owner;
		Entity* _target;

	public:
		static void BindActionUpdater(ActionUpdater& updater);
		Action();
		Action(ActionLogic fn);
		
		void setFunction(ActionLogic fn);
		void setOwner(Entity& owner);
		void setTarget(Entity& target);

		Entity& getOwner() const;
		Entity& getTarget() const;

		bool Activate();
		bool Deactivate();
		bool isActive() const;
		
	};
}

#endif //CORE_ACTION_ACTION_H_