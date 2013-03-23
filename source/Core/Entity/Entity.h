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
		typedef std::unordered_map<String, Action> ActionCache;
		typedef std::unordered_map<String, std::unique_ptr<IState>> StateCache;

		InstanceID _id;
		String _type;
		String _alias;
		
		ActionCache _actions;
		StateCache _states;

		Entity* _prototype;
		Form _form;

	public:
		//////////// CONSTRUCTORS ////////////
		Entity(InstanceID id);
		Entity(const Entity& rhs);
		Entity& operator=(const Entity& rhs);
		
		//////////// EXISTANCE CHECKS ////////////
		bool hasAction(const char* name);
		bool hasAction(const String& name);
		bool hasState(const char* name);
		bool hasState(const String& name);

		//////////// GETTERS ////////////
		InstanceID		getID() const;
		const String&	getType() const;
		const String&	getAlias() const;
		Form&			getForm();
		Action&			getAction(const char* name);
		Action&			getAction(const String& name);
		IState*			getState(const char* name);
		IState*			getState(const String& name);

		//////////// REMOVAL METHODS ////////////
		void ClearStates();
		bool RemoveState(const char* name);
		bool RemoveState(const String& name);
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
		bool Insert(const char* name, const Action& action);
		bool Insert(const String& name, const Action& action);
		bool Insert(const char* name, IState* state);
		bool Insert(const String& name, IState* state);
		bool Insert(const char* name, std::unique_ptr<IState> state);
		bool Insert(const String& name, std::unique_ptr<IState> state);

		//////////// TEMPLATED EASE METHODS ////////////
		template<typename T> T& getValue(const char* name)
		{
			auto it = _states.find(name);
			if(it == _states.end())
			{
				if(_prototype != nullptr)
				{
					_prototype->getValue<T>(name);
				}
				else
				{
					throw std::exception(String("Missing state ").append(name).append(" in Entity::getValue()").c_str());
				}
			}
			return it->second->as<T>();
		}

		template<typename T> T& getValue(const String& name)
		{
			return getValue<T>(name.c_str());
		}
	};
}

#endif //CORE_ENTITY_ENTITY_H_