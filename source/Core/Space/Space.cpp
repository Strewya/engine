//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include <stdafx.h>
	/*** personal header ***/
#include <Core/Space/Space.h>
	/*** C++ headers ***/
	/*** extra headers ***/
#include <Core/Entity/EntityPool.h>
	/*** end headers ***/

namespace Core
{
	Space::Space(InstanceID id)
		: m_id(id)
	{
	}

	Space::~Space()
	{
	}

	InstanceID Space::getID() const
	{
		return m_id;
	}

	bool Space::addEntity(InstanceID id)
	{
		return m_entities.insert(id).second;
	}

	bool Space::removeEntity(InstanceID id)
	{
		return m_entities.erase(id) != 0;
	}

	bool Space::findEntities(std::deque<InstanceID>& container, const EntityPool& pool, std::function<bool(const Entity&)> filter) const
	{
		for(auto id : m_entities)
		{
			if(pool.isAlive(id))
			{
				if(filter(pool.getInstanceRef(id)))
				{
					container.push_back(id);
				}
			}
		}
		return container.size() > 0;
	}
	
	Space::const_iterator Space::begin() const
	{
		return m_entities.begin();
	}

	Space::const_iterator Space::end() const
	{
		return m_entities.end();
	}

	Space::iterator Space::begin()
	{
		return m_entities.begin();
	}
	
	Space::iterator Space::end()
	{
		return m_entities.end();
	}

	void Space::clear()
	{
		m_entities.clear();
	}
}
