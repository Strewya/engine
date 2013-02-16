	/*** personal header ***/
#include "Core/Entity/Entity.h"
	/*** extra headers ***/
	/*** end headers ***/

namespace Core
{
	Entity::Entity(InstanceID id)
		: _id(id)
	{}

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
	
	Statemap& Entity::getStates()
	{
		return _states;
	}

	Actionmap& Entity::getActions()
	{
		return _actions;
	}

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


	/*
	Entity& Entity::setForm(const Form2D* f)
	{
		assert(f);
		return setForm(std::make_shared<Form2D>(f));
	}
	*/
	/*
	InstanceID Entity::_idCounter = 0;

	InstanceID Entity::_GenerateID()
	{
		return ++_idCounter;
	}

	Entity::Entity(const String& type)
		: _id(_GenerateID()), _parent(NULL), _location(NULL), _type(type), _alias(type+ToString(_id)), _form()
	{
		setForm(NullForm::Make(*this));
	}

	Entity::~Entity()
	{}

	// ------------------- UNSETTERS
	Entity& Entity::UnsetParent()
	{
		_parent = NULL;
		return *this;
	}

	Entity& Entity::UnsetLocation()
	{
		_location = NULL;
		return *this;
	}

	
	// ------------------------- SETTERS
	Entity& Entity::setParent(Entity* e)
	{
		assert(e);
		if(_parent == NULL)
		{
			_parent = e;
		}
		return *this;
	}

	Entity& Entity::setLocation(Space* loc)
	{
		assert(loc);
		if(_location == NULL)
		{
			_location = loc;
		}
		return *this;
	}

	Entity& Entity::setForm(spForm f)
	{
		assert(f.get());
		if(f != NULL)
		{
			if(_form != NULL)
			{
				_form->setContents(f->AcquireContents());
			}
			_form = f;
		}
		return *this;
	}

	


	// ------------------------------- GETTERS
	

	Entity* Entity::getParent()
	{
		return _parent;
	}

	Space* Entity::getLocation()
	{
		return _location;
	}

	

	

	Actionmap& Entity::getActions()
	{
		return _actions;
	}

	Form& Entity::getForm()
	{
		return *_form;
	}

	spForm Entity::AcquireForm()
	{
		return _form;
	}
	*/
}