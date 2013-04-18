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
		return _actions.find(name) != _actions.end();
	}

	bool Entity::hasAction(const String& name)
	{
		return _actions.find(name) != _actions.end();
	}

	bool Entity::hasState(const char* name, bool recursive)
	{
		bool found = _states.Contains(name);
		if(!found && recursive && _prototype != nullptr)
		{
			return _prototype->hasState(name, recursive);
		}
		return false;
	}

	bool Entity::hasState(const String& name, bool recursive)
	{
		return hasState(name.c_str(), recursive);
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

	Action& Entity::getAction(const char* name)
	{
		auto it = _actions.find(name);
		if(it == _actions.end())
		{
			String message = "Entity::getAction(): Attempted to acquire non-existant Action '";
			message += name;
			message += "'.";
			throw std::exception(message.c_str());
		}
		return it->second;
	}

	Action& Entity::getAction(const String& name)
	{
		return getAction(name.c_str());
	}

	State* Entity::getState(const char* name)
	{
		return _states.Retrieve(name);
	}

	State* Entity::getState(const String& name)
	{
		return _states.Retrieve(name);
	}
	
	/////////////////////////// REMOVAL METHODS ///////////////////////////

	void Entity::ClearActions()
	{
		_actions.clear();
	}

	bool Entity::RemoveAction(const char* name)
	{
		return _actions.erase(name) != 0;
	}

	bool Entity::RemoveAction(const String& name)
	{
		return _actions.erase(name) != 0;
	}

	void Entity::ClearStates()
	{
		_states.Clear();
	}

	bool Entity::RemoveState(const char* name)
	{
		return _states.Remove(name);
	}

	bool Entity::RemoveState(const String& name)
	{
		return _states.Remove(name);
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

	Entity& Entity::setPrototype(Entity& prototype)
	{
		_prototype = &prototype;
		return *this;
	}

	/////////////////////////// INSERT METHODS ///////////////////////////

	bool Entity::Insert(const char* name, const Action& action)
	{
		auto it = _actions.find(name);
		if(it == _actions.end())
		{
			it = _actions.emplace(name, action).first;
		}
		else
		{
			return false;
		}
		it->second.setOwner(*this);
		return true;
	}

	bool Entity::Insert(const String& name, const Action& action)
	{
		return Insert(name.c_str(), action);
	}

	bool Entity::Insert(const char* name, std::unique_ptr<State> state)
	{
		return _states.Insert(name, std::move(state));
	}

	bool Entity::Insert(const String& name, std::unique_ptr<State> state)
	{
		return _states.Insert(name, std::move(state));
	}
}