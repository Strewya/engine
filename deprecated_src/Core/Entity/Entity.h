#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include <Engine/Defines.h>
	/*** extra headers if needed (alphabetically ordered) ***/
#include <Core/State/State.h>
#include <Core/State/Statemap.h>
	/*** end header inclusion ***/

namespace Core
{
	/*
		Entity class
	*/
	class Entity
	{
	private:
		InstanceID m_id;
		std::string m_type;
		std::string m_alias;

		InstanceID m_prototype;
		
		Statemap m_states;

		//////////// PRIVATE NO IMPLEMENTATION CONSTRUCTORS ////////////

		Entity(const Entity& rhs);
		Entity(Entity&& rhs);
		Entity& operator=(const Entity& rhs);
		Entity& operator=(Entity&& rhs);

	public:
		//////////// CONSTRUCTORS ////////////

		Entity(InstanceID id);

		//////////// EXISTANCE CHECKS ////////////

		bool hasState(InstanceID id);
		
		//////////// GETTERS ////////////

		InstanceID		getID() const;
		const std::string&	getType() const;
		const std::string&	getAlias() const;
		InstanceID		getPrototype() const;
		StateRptr		getState(InstanceID id);
		template<typename T> T* getState();

		//////////// REMOVAL METHODS ////////////

		void clearStates();
		bool removeState(InstanceID id);

		//////////// SETTERS ////////////

		Entity& setAlias(const char* alias);
		Entity& setAlias(const std::string& alias);
		Entity& setType(const char* type);
		Entity& setType(const std::string& type);
		Entity& setPrototype(InstanceID prototype);
		
		//////////// INSERT METHODS ////////////

		template<typename T> typename T::Ref insert(std::unique_ptr<T> state);
		

	};



	template<typename T> T* Entity::getState()
	{
		StateRptr ptr = m_states.retrieve(T::Type);
		if(ptr != nullptr)
		{
			return ptr->cast<T>();
		}
		return nullptr;
	}

	template<typename T> typename T::Ref Entity::insert(std::unique_ptr<T> state)
	{
		typename T::Ref ref = *state;
		m_states.insert(std::move(state));
		return ref;
	}	
}
