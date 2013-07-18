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

	bool Entity::hasState(InstanceID id)
	{
		return _states.contains(id);
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
	
	Action* Entity::getAction(const char* name)
	{
		return _actions.Retrieve(name);
	}

	Action* Entity::getAction(const String& name)
	{
		return _actions.Retrieve(name);
	}

	StateRptr Entity::getState(InstanceID id)
	{
		return _states.retrieve(id);
	}
	
	/////////////////////////// REMOVAL METHODS ///////////////////////////

	void Entity::clearActions()
	{
		_actions.Clear();
	}

	bool Entity::removeAction(const char* name)
	{
		return _actions.Remove(name);
	}

	bool Entity::removeAction(const String& name)
	{
		return _actions.Remove(name);
	}

	void Entity::clearStates()
	{
		_states.clear();
	}

	bool Entity::removeState(InstanceID id)
	{
		return _states.destroy(id);
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

	bool Entity::insert(const char* name, std::unique_ptr<Action> action)
	{
		return _actions.Insert(name, std::move(action));
	}

	bool Entity::insert(const String& name, std::unique_ptr<Action> action)
	{
		return _actions.Insert(name, std::move(action));
	}

	bool Entity::insert(StateUptr state)
	{
		return _states.insert(std::move(state));
	}
}