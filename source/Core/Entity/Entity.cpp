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

	Entity::Entity(const Entity& rhs)
		: Statemap(rhs), _id(rhs._id), _type(rhs._type), _alias(rhs._alias), _form(rhs._form), _actions(rhs._actions)
	{
	}

	Entity::Entity(Entity&& rhs)
		: Statemap(rhs), _id(rhs._id), _type(rhs._type), _alias(rhs._alias), _form(rhs._form), _actions(rhs._actions)
	{
	}

	Entity& Entity::operator=(const Entity& rhs)
	{
		if(this != &rhs)
		{
			Statemap::operator=(rhs);
			_id = rhs._id;
			_type = rhs._type;
			_alias = rhs._alias;
			_form = rhs._form;
			_actions = rhs._actions;
		}
		return *this;
	}

	Entity& Entity::operator=(Entity&& rhs)
	{
		if(this != &rhs)
		{
			Statemap::operator=(std::move(rhs));
			_id = std::move(rhs._id);
			_type = std::move(rhs._type);
			_alias = std::move(rhs._alias);
			_form = std::move(rhs._form);
			_actions = std::move(rhs._actions);

			rhs._id = -1;
			rhs._type.clear();
			rhs._alias.clear();
			rhs._actions.clear();
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
}