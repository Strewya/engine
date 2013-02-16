#ifndef CORE_ENTITY_ENTITY_H_
#define CORE_ENTITY_ENTITY_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Core/Action/Actionmap.h"
#include "Core/Form/Form.h"
#include "Core/State/Statemap.h"
	/*** end header inclusion ***/

namespace Core
{
	/*
		Entity class
	*/
	class Entity
	{
	private:
		InstanceID _id;
		String _type;
		String _alias;
		
		Actionmap _actions;
		Statemap _states;
		Form _form;

	public:
		Entity(InstanceID id);

		InstanceID		getID() const;
		const String&	getType() const;
		const String&	getAlias() const;
		Form&			getForm();
		Statemap&		getStates();
		Actionmap&		getActions();

		Entity& setAlias(const char* alias);
		Entity& setAlias(const String& alias);
		Entity& setType(const char* type);
		Entity& setType(const String& type);
		Entity& setForm(const Form& form);
	};

	/*
	class Entity
	{
	private:
		static InstanceID _idCounter;
		static InstanceID _GenerateID();

		Entity(const String& type);
		Entity(Entity& rhs);
		Entity& operator=(Entity& rhs);

		
		Entity* _parent;
		Space* _location;

		

		Statemap _states;
		Actionmap _actions;
		std::shared_ptr<Form> _form;

	public:
		~Entity();
		static spEntity Make(const String& type = "Unknown") { return spEntity(new Entity(type)); }
		
		Entity& setParent(Entity* parent);
		Entity& setLocation(Space* location);
		
		Entity& setForm(spForm f);
		
		Entity& UnsetParent();
		Entity& UnsetLocation();
		
		
		Entity*			getParent();
		Space*			getLocation();
		
		
		Actionmap&		getActions();
		Form&			getForm();

		spForm			AcquireForm();
	};
	*/
}

#endif //CORE_ENTITY_ENTITY_H_