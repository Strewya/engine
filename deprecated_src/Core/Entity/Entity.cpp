	/*** precompiled header ***/
#include <stdafx.h>
	/*** personal header ***/
#include <Core/Entity/Entity.h>
	/*** extra headers ***/
	/*** end headers ***/

namespace Core
{
	/////////////////////////// CONSTRUCTORS ///////////////////////////

	Entity::Entity(InstanceID id)
		: m_id(id), m_prototype(0)
	{}

	/////////////////////////// EXISTANCE CHECKS ///////////////////////////

	bool Entity::hasState(InstanceID id)
	{
		return m_states.contains(id);
	}

	/////////////////////////// GETTERS ///////////////////////////

	InstanceID Entity::getID() const
	{
		return m_id;
	}

	InstanceID Entity::getPrototype() const
	{
		return m_prototype;
	}

	const std::string& Entity::getType() const
	{
		return m_type;
	}

	const std::string& Entity::getAlias() const
	{
		return m_alias;
	}
	
	StateRptr Entity::getState(InstanceID id)
	{
		return m_states.retrieve(id);
	}
	
	/////////////////////////// REMOVAL METHODS ///////////////////////////

	void Entity::clearStates()
	{
		m_states.clear();
	}

	bool Entity::removeState(InstanceID id)
	{
		return m_states.destroy(id);
	}

	/////////////////////////// SETTERS ///////////////////////////
	
	Entity& Entity::setAlias(const char* alias)
	{
		m_alias = alias;
		return *this;
	}

	Entity& Entity::setAlias(const std::string& alias)
	{
		m_alias = alias;
		return *this;
	}

	Entity& Entity::setType(const char* type)
	{
		m_type = type;
		return *this;
	}

	Entity& Entity::setType(const std::string& type)
	{
		m_type = type;
		return *this;
	}

	Entity& Entity::setPrototype(InstanceID prototype)
	{
		m_prototype = prototype;
		return *this;
	}

	/////////////////////////// INSERT METHODS ///////////////////////////
}