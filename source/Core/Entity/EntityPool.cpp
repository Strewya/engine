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
		: m_maxExpectedSize(maxExpectedSize), m_indexBits(0), m_indexMask(0), m_idCounter(0)
	{
		--maxExpectedSize;
		while(maxExpectedSize > 0)
		{
			++m_indexBits;
			m_indexMask <<= 1;
			m_indexMask |= 1;
			maxExpectedSize >>= 1;
		}
	}

	InstanceID EntityPool::newID(int index)
	{
		InstanceID id = ++m_idCounter;
		id <<= m_indexBits;
		id |= (index & m_indexMask);
		return id;
	}

	InstanceID EntityPool::getNewInstanceID()
	{
		return getNewInstanceRef().getID();
	}

	Entity& EntityPool::getNewInstanceRef()
	{
		uint32_t index;
		if(!m_availableSlots.empty())
		{
			index = m_availableSlots.front();
			m_availableSlots.pop_front();
		}
		else
		{
			index = m_pool.size();
			m_pool.emplace_back(nullptr);
		}
		InstanceID id = newID(index);
		m_pool[index].reset(new Entity(id));
		return *m_pool[index];
	}

	Entity* EntityPool::getInstancePtr(InstanceID id) const
	{
		uint32_t index = id & m_indexMask;
		return (index < m_pool.size() ? m_pool[index].get() : nullptr);
	}

	Entity& EntityPool::getInstanceRef(InstanceID id) const
	{
		uint32_t index = id & m_indexMask;
		if(index >= m_pool.size())
		{
			throw std::runtime_error("Attempt to get invalid Entity ID");
		}
		return *m_pool[index];
	}
	
	bool EntityPool::isAlive(InstanceID id) const
	{
		uint32_t index = id & m_indexMask;
		return (m_pool[index] != nullptr && m_pool[index]->getID() == id);
	}

	bool EntityPool::destroy(InstanceID id)
	{
		if(isAlive(id))
		{
			uint32_t index = id & m_indexMask;
			dispatchDestruction(*m_pool[index]);
			m_pool[index].reset(nullptr);
			m_availableSlots.push_back(index);
			return true;
		}
		return false;
	}

	void EntityPool::registerDestructionCallback(std::function<void(Entity&)> callback)
	{
		m_onDestroyCallbacks.push_back(callback);
	}

	void EntityPool::dispatchDestruction(Entity& e)
	{
		for(auto& callback : m_onDestroyCallbacks)
		{
			callback(e);
		}
	}
}
