//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Core/Entity/EntityPool.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Core/Entity/Entity.h"
	/*** end headers ***/

namespace Core
{
	EntityPool::EntityPool()
		: _idCounter(0)
	{}

	InstanceID EntityPool::_NewID()
	{
		return ++_idCounter;
	}

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

	Entity* EntityPool::getEntityByID(InstanceID id) const
	{
		for(auto& entityPtr : _entityList)
		{
			if(entityPtr->getID() == id)
			{
				return entityPtr.get();
			}
		}
		return nullptr;
	}

	Entity* EntityPool::getNewEntity()
	{
		if(!_availableSlots.empty())
		{
			int index = _availableSlots.front();
			_availableSlots.pop_front();
			_entityList[index].reset(new Entity(_NewID()));
			return _entityList[index].get();
		}
		_entityList.emplace_back(new Entity(_entityList.size()));
		return _entityList.back().get();
	}
}