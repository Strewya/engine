#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Core/Action/Action.h"
#include "Core/Action/Actionmap.h"
#include "Core/State/State.h"
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
		std::string _type;
		std::string _alias;

		Entity* _prototype;
		
		Actionmap _actions;
		Statemap _states;

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
		bool hasAction(const std::string& name);
		bool hasState(InstanceID id);
		
		//////////// GETTERS ////////////

		InstanceID		getID() const;
		const std::string&	getType() const;
		const std::string&	getAlias() const;
		Action*			getAction(const char* name);
		Action*			getAction(const std::string& name);
		StateRptr		getState(InstanceID id);
		template<typename T> T* getState();

		//////////// REMOVAL METHODS ////////////

		void clearActions();
		bool removeAction(const char* name);
		bool removeAction(const std::string& name);
		void clearStates();
		bool removeState(InstanceID id);

		//////////// SETTERS ////////////

		Entity& setAlias(const char* alias);
		Entity& setAlias(const std::string& alias);
		Entity& setType(const char* type);
		Entity& setType(const std::string& type);
		
		//////////// INSERT METHODS ////////////

		bool insert(const char* name, std::unique_ptr<Action> action);
		bool insert(const std::string& name, std::unique_ptr<Action> action);
		template<typename T> typename T::Ref insert(std::unique_ptr<T> state);
		

	};



	template<typename T> T* Entity::getState()
	{
		StateRptr ptr = _states.retrieve(T::Type);
		if(ptr != nullptr)
		{
			return ptr->cast<T>();
		}
		return nullptr;
	}

	template<typename T> typename T::Ref Entity::insert(std::unique_ptr<T> state)
	{
		typename T::Ref ref = *state;
		_states.insert(std::move(state));
		return ref;
	}	
}
