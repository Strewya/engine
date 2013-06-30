//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Core/Entity/EntityPool.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Core
{
	EntityPool::EntityPool(uint32_t maxExpectedSize)
		: _idCounter(0), _maxExpectedSize(maxExpectedSize), _indexMask(0), _indexBits(0)
	{
		--maxExpectedSize;
		while(maxExpectedSize > 0)
		{
			++_indexBits;
			_indexMask <<= 1;
			_indexMask |= 1;
			maxExpectedSize >>= 1;
		}
	}

	InstanceID EntityPool::_NewID(int index)
	{
		InstanceID id = ++_idCounter;
		id <<= _indexBits;
		id |= (index & _indexMask);
		return id;
	}

	Entity& EntityPool::NewInstance()
	{
		uint32_t index;
		if(!_availableSlots.empty())
		{
			index = _availableSlots.front();
			_availableSlots.pop_front();
		}
		else
		{
			index = _pool.size();
			_pool.emplace_back(nullptr);
		}
		InstanceID id = _NewID(index);
		_pool[index].reset(new Entity(id));
		return *_pool[index];
	}

	Entity& EntityPool::Retrieve(InstanceID id) const
	{
		uint32_t index = id & _indexMask;
		return *_pool[index];
	}
	
	bool EntityPool::IsAlive(InstanceID id) const
	{
		uint32_t index = id & _indexMask;
		return (_pool[index] != nullptr && _pool[index]->getID() == id);
	}

	bool EntityPool::Destroy(InstanceID id)
	{
		if(IsAlive(id))
		{
			uint32_t index = id & _indexMask;
			_pool[index].reset(nullptr);
			_availableSlots.push_back(index);
			return true;
		}
		return false;
	}
}