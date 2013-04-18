//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Core/Space/SpacePool.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Core/Space/Space.h"
	/*** end headers ***/

namespace Core
{
	SpacePool::SpacePool(uint maxExpectedSize)
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

	InstanceID SpacePool::_NewID(int index)
	{
		InstanceID id = ++_idCounter;
		id <<= _indexBits;
		id |= (index & _indexMask);
		return id;
	}

	Space& SpacePool::NewInstance()
	{
		uint index;
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
		_pool[index].reset(new Space(id));
		return *_pool[index];
	}

	Space& SpacePool::Retrieve(InstanceID id) const
	{
		uint index = id & _indexMask;
		return *_pool[index];
	}
	
	bool SpacePool::IsAlive(InstanceID id) const
	{
		uint index = id & _indexMask;
		return (_pool[index] != nullptr && _pool[index]->getID() == id);
	}

	bool SpacePool::Destroy(InstanceID id)
	{
		if(IsAlive(id))
		{
			uint index = id & _indexMask;
			_pool[index].reset(nullptr);
			_availableSlots.push_back(index);
			return true;
		}
		return false;
	}
}