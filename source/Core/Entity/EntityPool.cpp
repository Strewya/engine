//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Core/Entity/EntityPool.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Core
{
	EntityPool::EntityPool(uint maxExpectedEntities)
		: _idCounter(0), _maxExpectedEntities(maxExpectedEntities), _indexMask(0), _indexBits(0)
	{
		--maxExpectedEntities;
		while(maxExpectedEntities > 0)
		{
			++_indexBits;
			_indexMask <<= 1;
			_indexMask |= 1;
			maxExpectedEntities >>= 1;
		}
	}

	InstanceID EntityPool::_NewID(int index)
	{
		InstanceID id = ++_idCounter;
		id <<= _indexBits;
		id |= (index & _indexMask);
		return id;
	}

	Entity& EntityPool::NewEntity()
	{
		uint index;
		if(!_availableSlots.empty())
		{
			index = _availableSlots.front();
			_availableSlots.pop_front();
		}
		else
		{
			index = _entities.size();
			_entities.emplace_back(nullptr);
		}
		InstanceID id = _NewID(index);
		_entities[index].reset(new Entity(id));
		return *_entities[index];
	}

	Entity& EntityPool::GetEntity(InstanceID id) const
	{
		uint index = id & _indexMask;
		return *_entities[index];
	}
	
	bool EntityPool::IsAlive(InstanceID id) const
	{
		uint index = id & _indexMask;
		return (_entities[index] != nullptr && _entities[index]->getID() == id);
	}

	bool EntityPool::DestroyEntity(InstanceID id)
	{
		if(IsAlive(id))
		{
			uint index = id & _indexMask;
			_entities[index].reset(nullptr);
			_availableSlots.push_back(index);
			return true;
		}
		return false;
	}
}