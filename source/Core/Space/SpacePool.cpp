//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Core/Space/SpacePool.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Core
{
	SpacePool::SpacePool(uint32_t maxExpectedSize)
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

	InstanceID SpacePool::_newID(int index)
	{
		InstanceID id = ++_idCounter;
		id <<= _indexBits;
		id |= (index & _indexMask);
		return id;
	}

	InstanceID SpacePool::getNewInstance(Space** outSpace)
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
		_pool[index].reset(new Space(id));
		if(outSpace != nullptr)
		{
			*outSpace = _pool[index].get();
		}
		return id;
	}

	InstanceID SpacePool::getNamedInstance(const char* name, Space** outSpace)
	{
		InstanceID id = NOT_FOUND;
		auto it = _names.find(name);
		if(it == _names.end())
		{
			id = getNewInstance(outSpace);
			_names.emplace(std::make_pair(name, id));
		}
		else
		{
			id = it->second;
		}
		return id;
	}

	Space* SpacePool::getInstance(InstanceID id) const
	{
		uint32_t index = id & _indexMask;
		return (index < _pool.size() ? _pool[index].get() : nullptr);
	}
	
	Space* SpacePool::getInstance(const char* name) const
	{
		auto it = _names.find(name);
		return (it != _names.end() ? _pool[it->second].get() : nullptr);
	}

	bool SpacePool::isAlive(InstanceID id) const
	{
		uint32_t index = id & _indexMask;
		return (_pool[index] != nullptr && _pool[index]->getID() == id);
	}

	bool SpacePool::isNameInUse(const char* name) const
	{
		return _names.find(name) != _names.end();
	}

	bool SpacePool::destroy(InstanceID id)
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

	bool SpacePool::destroy(const char* name)
	{
		auto it = _names.find(name);
		if(it != _names.end())
		{
			destroy(it->second);
			_names.erase(it);
			return true;
		}
		return false;
	}

	bool SpacePool::attachName(const char* name, InstanceID id)
	{
		auto it = _names.find(name);
		if(it == _names.end() && isAlive(id))
		{
			_names.emplace(std::make_pair(name, id));
			return true;
		}
		return false;
	}

	bool SpacePool::detachName(const char* name, InstanceID id)
	{
		auto it = _names.find(name);
		if(it != _names.end() && it->second == id)
		{
			_names.erase(it);
			return true;
		}
		return false;
	}
}
