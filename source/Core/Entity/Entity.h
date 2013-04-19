#ifndef CORE_ENTITY_ENTITY_H_
#define CORE_ENTITY_ENTITY_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Core/Action/Action.h"
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

		Entity* _prototype;
		
		Actionmap _actions;
		Statemap _states;
		Form _form;

		//////////// PRIVATE NO IMPLEMENTATION CONSTRUCTORS ////////////

		Entity(const Entity& rhs);
		Entity(Entity&& rhs);
		Entity& operator=(const Entity& rhs);
		Entity& operator=(Entity&& rhs);

	public:
		//////////// CONSTRUCTORS ////////////

		Entity(InstanceID id);

		//////////// EXISTANCE CHECKS ////////////

		bool hasAction(const char* name);
		bool hasAction(const String& name);
		bool hasState(const char* name, bool recursive = false);
		bool hasState(const String& name, bool recursive = false);
		
		//////////// GETTERS ////////////

		InstanceID		getID() const;
		const String&	getType() const;
		const String&	getAlias() const;
		Form&			getForm();
		Action*			getAction(const char* name);
		Action*			getAction(const String& name);
		State*			getState(const char* name);
		State*			getState(const String& name);

		//////////// REMOVAL METHODS ////////////

		void ClearActions();
		bool RemoveAction(const char* name);
		bool RemoveAction(const String& name);
		void ClearStates();
		bool RemoveState(const char* name);
		bool RemoveState(const String& name);

		//////////// SETTERS ////////////

		Entity& setAlias(const char* alias);
		Entity& setAlias(const String& alias);
		Entity& setType(const char* type);
		Entity& setType(const String& type);
		Entity& setPrototype(Entity& prototype);
		
		//////////// INSERT METHODS ////////////

		bool Insert(const char* name, std::unique_ptr<Action> action);
		bool Insert(const String& name, std::unique_ptr<Action> action);
		bool Insert(const char* name, std::unique_ptr<State> state);
		bool Insert(const String& name, std::unique_ptr<State> state);
	};
}

#endif //CORE_ENTITY_ENTITY_H_