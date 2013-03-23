//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Core/Entity/EntityPool.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Core/Entity/Entity.h"
	/*** end headers ***/

namespace Core
{
	Entity* EntityPool::getEntityByAlias(const String& alias) const
	{
		for(auto& entityPtr : _entityList)
		{
			if(entityPtr->getAlias().compare(alias) == 0)
			{
				return entityPtr.get();
			}
		}
		return nullptr;
	}

	Entity* EntityPool::getNewEntity()
	{
		for(auto it = _entityList.begin(); it != _entityList.end(); ++it)
		{
			if(it->get() == nullptr)
			{
				it->reset(new Entity(it - _entityList.begin()));
				return it->get();
			}
		}
		_entityList.emplace_back(new Entity(_entityList.size()));
		return _entityList.back().get();
	}
}