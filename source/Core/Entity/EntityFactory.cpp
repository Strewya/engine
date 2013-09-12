	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Core/Entity/EntityFactory.h"
	/*** extra headers ***/
#include "Core/Entity/Entity.h"
	/*** end headers ***/

namespace Core
{
	EntityFactory::EntityFactory(GameContext& owner)
		: m_ownerContext(owner)
	{}

	bool EntityFactory::createEntityType(const std::string& type, Entity& target) const
	{
		bool success = false;
		auto it = m_creators.find(type);
		if(it != m_creators.end() && it->second(m_ownerContext, target))
		{
			target.setType(type);
			success = true;
		}
		return success;
	}

	bool EntityFactory::registerConstructor(const std::string& typeName, const EntityCreator_t& creator)
	{
		return m_creators.insert(std::make_pair(typeName, creator)).second;
	}
}
