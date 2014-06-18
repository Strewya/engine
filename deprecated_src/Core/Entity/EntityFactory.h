#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include <Engine/Defines.h>
#include <functional>
#include <unordered_map>
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Core
{
	class GameContext;
	class Entity;

	class EntityFactory
	{
	public:
		typedef std::function<bool(GameContext&, Entity&)> EntityCreator_t;
		EntityFactory(GameContext& owningContext);
		bool createEntityType(const std::string& type, Entity& target) const;
		bool registerConstructor(const std::string& typeName, const EntityCreator_t& creator);
	private:
		std::unordered_map<std::string, EntityCreator_t> m_creators;
		GameContext& m_ownerContext;
	};
}