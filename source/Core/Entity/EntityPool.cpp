//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Core/Entity/EntityPool.h"
	/*** C++ headers ***/
#include <stdexcept>
	/*** extra headers ***/
	/*** end headers ***/

namespace Core
{
	EntityPool::EntityPool(uint32_t maxExpectedSize)
		: _maxExpectedSize(maxExpectedSize), _indexBits(0), _indexMask(0), _idCounter(0)
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

	InstanceID EntityPool::_newID(int index)
	{
		InstanceID id = ++_idCounter;
		id <<= _indexBits;
		id |= (index & _indexMask);
		return id;
	}

	InstanceID EntityPool::getNewInstanceID()
	{
		return getNewInstanceRef().getID();
	}

	Entity& EntityPool::getNewInstanceRef()
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
		InstanceID id = _newID(index);
		_pool[index].reset(new Entity(id));
		return *_pool[index];
	}

	Entity* EntityPool::getInstancePtr(InstanceID id) const
	{
		uint32_t index = id & _indexMask;
		return (index < _pool.size() ? _pool[index].get() : nullptr);
	}

	Entity& EntityPool::getInstanceRef(InstanceID id) const
	{
		uint32_t index = id & _indexMask;
		if(index >= _pool.size())
		{
			throw std::runtime_error("Attempt to get invalid Entity ID");
		}
		return *_pool[index];
	}
	
	bool EntityPool::isAlive(InstanceID id) const
	{
		uint32_t index = id & _indexMask;
		return (_pool[index] != nullptr && _pool[index]->getID() == id);
	}

	bool EntityPool::destroy(InstanceID id)
	{
		if(isAlive(id))
		{
			uint32_t index = id & _indexMask;
			_pool[index].reset(nullptr);
			_availableSlots.push_back(index);
			return true;
		}
		return false;
	}
}
