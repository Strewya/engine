#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
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
	private:
		std::unordered_map<std::string, std::function<bool(GameContext&, Entity&)>> _creators;
		GameContext& _ownerContext;
	public:
		EntityFactory(GameContext& owningContext);
		bool createEntityType(const std::string& type, Entity& target) const;
		bool registerConstructor(const std::string& typeName, const std::function<bool(GameContext&, Entity&)>& creator);
	};
}