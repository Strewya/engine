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
		std::unordered_map<String, std::function<bool(GameContext&, Entity&)>> _creators;
		GameContext& _ownerContext;
	public:
		EntityFactory(GameContext& owningContext);
		bool CreateEntityType(const String& type, Entity& target) const;
		bool RegisterConstructor(const String& typeName, const std::function<bool(GameContext&, Entity&)>& creator);
	};
}