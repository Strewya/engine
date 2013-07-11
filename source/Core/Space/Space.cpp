//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Core/Space/Space.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Core/Entity/EntityPool.h"
	/*** end headers ***/

namespace Core
{
	Space::Space(InstanceID id)
		: _id(id), _bounds()
	{
	}

	Space::Space(InstanceID id, const Util::Rect& bounds)
		: _id(id), _bounds(bounds)
	{
	}

	Space::Space(InstanceID id, const Util::Vec2& pos, const Util::Vec2& size)
		: _id(id), _bounds(pos, size)
	{
	}

	Space::~Space()
	{
	}

	InstanceID Space::getID() const
	{
		return _id;
	}

	const Util::Rect& Space::getBounds() const
	{
		return _bounds;
	}

	void Space::setBounds(const Util::Rect& bounds)
	{
		_bounds = bounds;
	}

	void Space::setBounds(const Util::Vec2& pos, const Util::Vec2& size)
	{
		_bounds = Util::Rect(pos, size);
	}

	bool Space::AddEntity(InstanceID id)
	{
		return _entities.insert(id).second;
	}

	bool Space::RemoveEntity(InstanceID id)
	{
		return _entities.erase(id) != 0;
	}

	bool Space::FindEntities(std::deque<InstanceID>& container, const EntityPool& pool, std::function<bool(const Entity&)> filter) const
	{
		for(auto id : _entities)
		{
			if(pool.isAlive(id))
			{
				if(filter(*pool.getInstance(id)))
				{
					container.push_back(id);
				}
			}
		}
		return container.size() > 0;
	}
	
	Space::iterator Space::begin() const
	{
		return _entities.begin();
	}
	
	Space::iterator Space::end() const
	{
		return _entities.end();
	}
}