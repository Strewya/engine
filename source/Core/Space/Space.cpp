//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Core/Space/Space.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Core/Entity/EntityPool.h"
	/*** end headers ***/

namespace Core
{
	Space::Space(InstanceID id)
		: _id(id)
	{
	}

	Space::~Space()
	{
	}

	InstanceID Space::getID() const
	{
		return _id;
	}

	bool Space::addEntity(InstanceID id)
	{
		return _entities.insert(id).second;
	}

	bool Space::removeEntity(InstanceID id)
	{
		return _entities.erase(id) != 0;
	}

	bool Space::findEntities(std::deque<InstanceID>& container, const EntityPool& pool, std::function<bool(const Entity&)> filter) const
	{
		for(auto id : _entities)
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
		return _entities.begin();
	}

	Space::const_iterator Space::end() const
	{
		return _entities.end();
	}

	Space::iterator Space::begin()
	{
		return _entities.begin();
	}
	
	Space::iterator Space::end()
	{
		return _entities.end();
	}
}
