#ifndef CORE_ENTITY_ENTITY_H_
#define CORE_ENTITY_ENTITY_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Core/Action/Action.h"
#include "Core/Form/Form.h"
#include "Core/State/Statemap.h"
	/*** end header inclusion ***/

namespace Core
{
	/*
		Entity class
	*/
	class Entity : public Statemap
	{
	private:
		typedef std::unordered_map<String, Action> ActionCache;
		
		InstanceID _id;
		String _type;
		String _alias;
		
		ActionCache _actions;
		Form _form;

	public:
		//////////// CONSTRUCTORS ////////////

		Entity(InstanceID id);
		Entity(const Entity& rhs);
		Entity(Entity&& rhs);
		Entity& operator=(const Entity& rhs);
		Entity& operator=(Entity&& rhs);
		
		//////////// EXISTANCE CHECKS ////////////

		bool hasAction(const char* name);
		bool hasAction(const String& name);
		
		//////////// GETTERS ////////////

		InstanceID		getID() const;
		const String&	getType() const;
		const String&	getAlias() const;
		Form&			getForm();
		Action&			getAction(const char* name);
		Action&			getAction(const String& name);
		
		//////////// REMOVAL METHODS ////////////

		void ClearActions();
		bool RemoveAction(const char* name);
		bool RemoveAction(const String& name);

		//////////// SETTERS ////////////

		Entity& setAlias(const char* alias);
		Entity& setAlias(const String& alias);
		Entity& setType(const char* type);
		Entity& setType(const String& type);
		Entity& setPrototype(Entity& prototype);
		Entity& setForm(const Form& form);
		Entity& setForm(Form&& form);
		
		//////////// INSERT METHODS ////////////

		using Statemap::Insert;
		bool Insert(const char* name, const Action& action);
		bool Insert(const String& name, const Action& action);
	};
}

#endif //CORE_ENTITY_ENTITY_H_