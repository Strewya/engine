	/*** personal header ***/
#include "Core/Entity/Entity.h"
	/*** extra headers ***/
	/*** end headers ***/

namespace Core
{
	/////////////////////////// CONSTRUCTORS ///////////////////////////

	Entity::Entity(InstanceID id)
		: _id(id), _prototype(nullptr)
	{}

	Entity::Entity(const Entity& rhs)
		: _id(rhs._id), _type(rhs._type), _alias(rhs._alias), _prototype(rhs._prototype), _form(rhs._form), _actions(rhs._actions)
	{
		for(auto& pair : rhs._states)
		{
			Insert(pair.first, pair.second->Clone());
		}
	}

	Entity& Entity::operator=(const Entity& rhs)
	{
		if(this != &rhs)
		{
			_id = rhs._id;
			_type = rhs._type;
			_alias = rhs._alias;
			_prototype = rhs._prototype;
			_form = rhs._form;
			_actions = rhs._actions;
			for(auto& pair : rhs._states)
			{
				Insert(pair.first, pair.second->Clone());
			}
		}
		return *this;
	}

	/////////////////////////// EXISTANCE CHECKS ///////////////////////////

	bool Entity::hasAction(const char* name)
	{
		return _actions.find(name) != _actions.end();
	}

	bool Entity::hasAction(const String& name)
	{
		return _actions.find(name) != _actions.end();
	}

	bool Entity::hasState(const char* name)
	{
		return _states.find(name) != _states.end();
	}

	bool Entity::hasState(const String& name)
	{
		return _states.find(name) != _states.end();
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
	
	IState* Entity::getState(const char* name)
	{
		auto it = _states.find(name);
		if(it == _states.end())
		{
			if(_prototype)
			{
				return _prototype->getState(name);
			}
			return nullptr;
		}
		return it->second.get();
	}

	IState* Entity::getState(const String& name)
	{
		return getState(name.c_str());
	}

	/////////////////////////// REMOVAL METHODS ///////////////////////////

	void Entity::ClearStates()
	{
		_states.clear();
	}

	bool Entity::RemoveState(const char* name)
	{
		return _states.erase(name) != 0;
	}

	bool Entity::RemoveState(const String& name)
	{
		return _states.erase(name) != 0;
	}

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
	
	Entity& Entity::setForm(const Form& form)
	{
		_form = form;
		return *this;
	}

	Entity& Entity::setForm(Form&& form)
	{
		_form = std::move(form);
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

	bool Entity::Insert(const char* name, IState* state)
	{
		return Insert(name, std::unique_ptr<IState>(state));
	}

	bool Entity::Insert(const String& name, IState* state)
	{
		return Insert(name.c_str(), std::unique_ptr<IState>(state));
	}
	
	bool Entity::Insert(const char* name, std::unique_ptr<IState> state)
	{
		auto it = _states.find(name);
		if(it == _states.end())
		{
			return _states.emplace(std::make_pair(name, std::move(state))).second;
		}
		it->second.swap(state);
		return true;
	}
	
	bool Entity::Insert(const String& name, std::unique_ptr<IState> state)
	{
		return Insert(name.c_str(), std::move(state));
	}
}