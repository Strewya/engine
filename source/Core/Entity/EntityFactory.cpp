	/*** personal header ***/
#include "Core/Entity/EntityFactory.h"
	/*** extra headers ***/
	/*** end headers ***/

namespace Core
{
	EntityFactory::EntityFactory(GameContext& owner)
		: _ownerContext(owner)
	{}

	bool EntityFactory::CreateEntityType(const String& type, Entity& target) const
	{
		auto& it = _creators.find(type);
		if(it == _creators.end())
		{
			return false;
		}

		return it->second(_ownerContext, target);
	}

	bool EntityFactory::RegisterConstructor(const String& typeName, const std::function<bool(GameContext&, Entity&)>& creator)
	{
		return _creators.insert(std::make_pair(typeName, creator)).second;
	}
}