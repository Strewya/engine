	/*** personal header ***/
#include "Core/Entity/Entity.h"
	/*** extra headers ***/
	/*** end headers ***/

namespace Core
{
	/////////////////////////// CONSTRUCTORS ///////////////////////////

	Entity::Entity(InstanceID id)
		: _id(id)
	{}

	/////////////////////////// EXISTANCE CHECKS ///////////////////////////

	bool Entity::hasAction(const char* name)
	{
		return _actions.Contains(name);
	}

	bool Entity::hasAction(const String& name)
	{
		return _actions.Contains(name);
	}

	bool Entity::hasState(const char* name)
	{
		return _states.contains(name);
	}

	bool Entity::hasState(const String& name)
	{
		return hasState(name.c_str());
	}

	/////////////////////////// GETTERS ///////////////////////////

	InstanceID Entity::getID() const
	{
		return _id;
	}

	const String& Entity::getType() const
	{
		return _type;
	}

	const String& Entity::getAlias() const
	{
		return _alias;
	}
	
	Form& Entity::getForm()
	{
		return _form;
	}

	Action* Entity::getAction(const char* name)
	{
		return _actions.Retrieve(name);
	}

	Action* Entity::getAction(const String& name)
	{
		return _actions.Retrieve(name);
	}

	StateRptr Entity::getState(const char* name)
	{
		return _states.retrieve(name);
	}

	StateRptr Entity::getState(const String& name)
	{
		return _states.retrieve(name);
	}
	
	/////////////////////////// REMOVAL METHODS ///////////////////////////

	void Entity::ClearActions()
	{
		_actions.Clear();
	}

	bool Entity::RemoveAction(const char* name)
	{
		return _actions.Remove(name);
	}

	bool Entity::RemoveAction(const String& name)
	{
		return _actions.Remove(name);
	}

	void Entity::ClearStates()
	{
		_states.clear();
	}

	bool Entity::RemoveState(const char* name)
	{
		return _states.destroy(name);
	}

	bool Entity::RemoveState(const String& name)
	{
		return _states.destroy(name);
	}

	/////////////////////////// SETTERS ///////////////////////////
	
	Entity& Entity::setAlias(const char* alias)
	{
		_alias = alias;
		return *this;
	}

	Entity& Entity::setAlias(const String& alias)
	{
		_alias = alias;
		return *this;
	}

	Entity& Entity::setType(const char* type)
	{
		_type = type;
		return *this;
	}

	Entity& Entity::setType(const String& type)
	{
		_type = type;
		return *this;
	}

	/////////////////////////// INSERT METHODS ///////////////////////////

	bool Entity::Insert(const char* name, std::unique_ptr<Action> action)
	{
		action->setOwner(*this);
		return _actions.Insert(name, std::move(action));
	}

	bool Entity::Insert(const String& name, std::unique_ptr<Action> action)
	{
		action->setOwner(*this);
		return _actions.Insert(name, std::move(action));
	}

	bool Entity::Insert(const char* name, StateUptr state)
	{
		return _states.insert(name, std::move(state));
	}

	bool Entity::Insert(const String& name, StateUptr state)
	{
		return _states.insert(name, std::move(state));
	}
}